#ifndef __PARSER_H
#define __PARSER_H

#ifndef USE_HASHING
#define USE_HASHING
#endif

#include "ds/queue.h"
#include "ds/stack.h"
#include "basic_hash.h"

struct binary_operator{
    char op;
    int8_t pred:7;
    enum {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT
    }assoc;
};

struct operator {
    enum {
        BINARY,
        FUNCTION,
        PAREN
    }operator_type;
    union {
        struct binary_operator* binary_op;
#ifdef USE_HASHING
        int index_func;
#else
        char* function;
#endif
        enum {
            CLOSE,
            OPEN
        }parenthesis;
    }payload;
};

struct parser_output {
    union {
        int i;
        float f;
        char c;
        const char* str;
    }payload;
    enum {
        DATA,
        OPERATOR,
        FUNC
    }payload_type;
    enum {
        NONE,
        INT,
        FLOAT
    }payload_scalar_type;
};



queue_t parse(const char*);

#ifdef EXPOSE_PARSER_UTIL
extern const char* functions[];
extern size_t functionHash[];
int toFunction(const char* __restrict__, char**);
int hashArray(const char**, size_t*, int);
#endif

#endif