#ifndef __PARSER_H
#define __PARSER_H

#include "ds/queue.h"
#include "ds/stack.h"
#include "basic_hash.h"

struct operator {
    char op;
};

struct parser_output {
    union {
        int i;
        float f;
        char* c;
    }payload;
    enum {
        DATA,
        OPERATOR
    }payload_type;
    enum {
        NONE,
        INT,
        FLOAT
    }payload_scalar_type;
};

struct parser_output* parse(const char*);

#endif