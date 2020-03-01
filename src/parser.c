#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef DEBUG
#include <stdio.h>
#define debug_act(expr) expr
#else
#define debug_act(...)
#endif

#define consume_forall(x,c) ({ \
    while(*x && *x == c)++x;   \
})


#ifndef EXPOSE_PARSER_UTIL
#define INLINE_PARSER static inline
#else
#define INLINE_PARSER
#endif

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

#define MAX_FUNC_LENGTH 4

#ifdef USE_HASHING
const char* functions[] = {"sin", "cos", "tan", "ln"};

int hashed = 0;
size_t functionHash[ARRAY_SIZE(functions)];
INLINE_PARSER int hashArray(const char** func, size_t* dest, int n) {
    for (int i = 0 ; i < n ; ++i)dest[i] = hash(func[i]);
    return 1;
}
#endif

INLINE_PARSER int toFunction(const char* __restrict__ nptr, char** __restrict__ endptr) {
#ifdef USE_HASHING
    size_t i = 0;
    if (!hashed)
        hashed = hashArray(functions, functionHash, sizeof(functions)/sizeof(const char*));
    if (endptr == NULL){
        static char* _endptr;
        *endptr = _endptr;
    }
    *endptr = (char*)nptr;
    
    while (*endptr && ((char*)*endptr - (char*)nptr) <= MAX_FUNC_LENGTH) {
        size_t hashedVal = hash_n(nptr, (uintptr_t)((char*)*endptr-(char*)nptr+1)); // start with 1 char
        for (i = 0; i < ARRAY_SIZE(functions) ; ++i)
            if (hashedVal == functionHash[i])
                goto ext;
        ++*endptr;
    }
    *endptr = (char*)nptr;
    return 0;
ext:
    return i;
#else
#error not implemented
#endif
}

struct binary_operator operators[] = {{.op='*', .pred=4, .assoc=LEFT_TO_RIGHT},{.op='/', .pred=4, .assoc=LEFT_TO_RIGHT},{.op='%', .pred=4, .assoc=LEFT_TO_RIGHT}\
                                            ,{.op='+', .pred=3, .assoc=LEFT_TO_RIGHT},{.op='-', .pred=3, .assoc=LEFT_TO_RIGHT}\
};

INLINE_PARSER struct operator* apropos_operator(char op) {

    unsigned long i;
    for (i = 0 ; i < ARRAY_SIZE(operators) && operators[i].op != op; ++i);
    if (i < ARRAY_SIZE(operators))
        return &(struct operator){.operator_type=BINARY, .payload.binary_op=operators+i};
    return NULL;
}




// TODO:
// Deal with functions
// Hashing is inefficient, remove it

queue_t parse(const char* expr) {
    union {
        int i;
        float f;
        char* c;
        struct parser_output po;
        struct operator op;
        struct parser_output *p_po;
        struct operator *p_op;
    }buffer,_buffer;
    char* endptr = (char*)expr;
    stack_t operator_stack;
    queue_t output_queue;
    stack_create(&operator_stack, malloc(1024), 1024);
    queue_create(&output_queue, malloc(1024), 1024);
    while (*(expr = endptr)) {
        //dump_queue(stdout, &output_queue, struct parser_output, "%p", __front);
        // deal with signs
        // this is the case where plus and minus are treated as signs
        debug_act(printf("expr: %p\n", expr));
        buffer.i = strtol(expr, &endptr, 10);
        if (expr != endptr) {
            queue_push_back(&output_queue, ((struct parser_output){.payload.i = buffer.i, .payload_type=DATA, .payload_scalar_type=INT}) , struct parser_output);
            continue;
        }
        buffer.f = strtod(expr, &endptr);
        if (expr != endptr) {
            queue_push_back(&output_queue, ((struct parser_output){.payload.f = buffer.f, .payload_type=DATA, .payload_scalar_type=FLOAT}), struct parser_output);
            continue;
        }

        buffer.i = toFunction(expr, &endptr);
        if (expr != endptr) {
#ifdef USE_HASHING
            stack_push(&operator_stack, ((struct operator){.operator_type=FUNCTION, .payload.index_func=buffer.i}), struct operator);
#else
#error not implementented
#endif
        }
        consume_forall(expr, ' ');
        if ((buffer.p_op=apropos_operator(*expr), !!(buffer.p_op))) {
                     /* And the operator on top is not a left paren */
            _buffer.p_op = stack_back(&operator_stack, struct operator);  
            while  ( _buffer.p_op && (( !(_buffer.p_op->operator_type == PAREN && _buffer.p_op->payload.parenthesis == OPEN) )                                                    \
                    && /* There's a function on top of the operator stack*/                                                                               \
                    ( (_buffer.p_op->operator_type == FUNCTION)                   \
                    || /* There's an operator on top of the operator stack with greater precedence */                                                     \
                      (_buffer.p_op->payload.binary_op->pred > buffer.p_op->payload.binary_op->pred)                                                         \
                    || /* The operator on top of the operator stack has equal precedence and left-assoc*/                                                 \
                      ((_buffer.p_op->payload.binary_op->pred == buffer.p_op->payload.binary_op->pred) && buffer.p_op->payload.binary_op->assoc == LEFT_TO_RIGHT)))
                ){
                    dump_stack(stdout, &operator_stack, struct operator, "%c", __top->payload.binary_op->op);
                    struct operator* oper = stack_pop(&operator_stack, struct operator);
                    if (!oper)
                        break;
                    switch (oper->operator_type) {
                        case FUNCTION : 
                            queue_push_back(&output_queue, ((struct parser_output){.payload_type=FUNC, .payload.str=functions[oper->payload.index_func]}), struct parser_output);
                            break;
                        case BINARY : 
                            queue_push_back(&output_queue, ((struct parser_output){.payload_type=OPERATOR, .payload.c=oper->payload.binary_op->op}), struct parser_output);
                            break;
                        case PAREN:
                        default : {
                            // handle error
                        }
                    }
            }
            stack_push(&operator_stack, *buffer.p_op, struct operator);
            consume_forall(endptr, ' ');
            ++endptr;
        }
        else if (*expr == '(') {
            stack_push(&operator_stack, (((struct operator){.operator_type=PAREN,.payload.parenthesis=OPEN})), struct operator);
            consume_forall(endptr, ' ');
            ++endptr;
        } else if (*expr == ')') {
            dump_queue(stdout, &output_queue, struct parser_output, "%d", __front->payload.i);
            dump_stack(stdout, &operator_stack, struct operator, "%p", __top);
            buffer.p_op = stack_back(&operator_stack, struct operator);
            while (buffer.p_op && !(buffer.p_op->operator_type==PAREN && buffer.p_op->payload.parenthesis==OPEN) ) {
                struct operator* oper = stack_pop(&operator_stack, struct operator);
                if (!oper)
                    break;
                switch (oper->operator_type) {
                    case FUNCTION : 
                        queue_push_back(&output_queue, ((struct parser_output){.payload_type=FUNC, .payload.str=functions[oper->payload.index_func]}), struct parser_output);
                        break;
                    case BINARY : 
                        queue_push_back(&output_queue, ((struct parser_output){.payload_type=OPERATOR, .payload.c=oper->payload.binary_op->op}), struct parser_output);
                        break;
                    case PAREN:
                    default : {
                        // handle error
                    }
                }
            }
            if ((buffer.p_op = stack_back(&operator_stack, struct operator), buffer.p_op->operator_type == PAREN && buffer.p_op->payload.parenthesis == OPEN)) {
                stack_pop(&operator_stack, struct operator);
            }
            consume_forall(endptr, ' ');
            ++endptr;
        }
    }
    while ((buffer.p_op = stack_pop(&operator_stack, struct operator))) {
        switch (buffer.p_op->operator_type) {
            case FUNCTION : 
                queue_push_back(&output_queue, ((struct parser_output){.payload_type=FUNC, .payload.str=functions[buffer.p_op->payload.index_func]}), struct parser_output);
                break;
            case BINARY : 
                queue_push_back(&output_queue, ((struct parser_output){.payload_type=OPERATOR, .payload.c=buffer.p_op->payload.binary_op->op}), struct parser_output);
                break;
            case PAREN:
            default : {

            }
                // handle error
        }
    }
    

    return output_queue;
}