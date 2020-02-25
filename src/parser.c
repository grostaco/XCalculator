#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef USE_HASHING
#define USE_HASHING
#endif

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

struct parser_output* parse(const char* expr) {
    union {
        int i;
        float f;
        char* c;
    }buffer;
    char* endptr = (char*)expr;
    stack_t operator_stack;
    queue_t output_queue;
    stack_create(&operator_stack, malloc(1024), 1024);
    queue_create(&output_queue, malloc(1024), 1024);
    while (*(expr = endptr)) {
        if ((buffer.i = strtol(expr, &endptr, 10)) && expr != endptr) {
            queue_push_back(&output_queue, ((struct parser_output){.payload.i = buffer.i, .payload_type=DATA, .payload_scalar_type=INT}) , struct parser_output);
            continue;
        }
        if ((buffer.f = strtod(expr, &endptr) && expr != endptr)) {
            queue_push_back(&output_queue, ((struct parser_output){.payload.f = buffer.f, .payload_type=DATA, .payload_scalar_type=FLOAT}), struct parser_output);
            continue;
        }   

    }
    return NULL;
}

#define MAX_FUNC_LENGTH 4


int hashed = 0;
const char* functions[] = {"sin", "cos", "tan", "sinh", "cosh", "tanh", "ln", 0};
size_t functionHash[ARRAY_SIZE(functions)];
int isFunction(const char* __restrict__ nptr, char** __restrict__ endptr) {
#ifdef USE_HASHING
    if (!hashed)
        hashed = hashArray(functions, functionHash, sizeof(functions)/sizeof(const char*));
    for (int i = 0 ; i < ARRAY_SIZE(functions) ; ++i) {
        for (int j = 0 ; j < ARRAY_SIZE(functions) ; ++j) {

        }
    } 
#else

#endif
}

int hashArray(const char** func, size_t** dest, int n) {
    while (n--)dest[n] = hash(func[n]);
    return 1;
}
