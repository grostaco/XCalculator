#ifndef __STACK_H
#define __STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct stack {
    void* top;
    void* curr;
    uint32_t threshold;
}stack_t;

#ifdef DEBUG
#include <stdio.h>
#define dump_stack(stream, _stack, type, spec, args) ({     \
    const type* __top = (_stack)->top;                  \
    while (__top < (type*)((_stack)->curr)) {                     \
        fprintf(stream, "dump_stack:%s:%d:"spec"\n", __FILE__, __LINE__, args);\
        __top++;                                   \
    }                                                       \
})

#else
#define dump_queue(...)
#endif

#define stack_create(_stack, block, size)  ({                                  \
    (_stack)->top = (_stack)->curr = block;                                    \
    (_stack)->threshold = size;                                                \
}) 

#define stack_push(_stack, element, type)  ({                                 \
    const void* __current = (_stack)->curr;                                   \
    ((_stack)->curr + sizeof(type) ==                                         \
    (_stack)->top+(_stack)->threshold) ?                                      \
    (NULL)                                        :                           \
    ((_stack)->curr += sizeof(type)), *((type*)__current)=                    \
    element, (__current);                                                     \
})

#define stack_pop(_stack, type) ({                                            \
    ((_stack)->curr - sizeof(type) <  (_stack)->top)   ?                      \
    (NULL)                                                :                   \
    (((_stack)->curr -= sizeof(type)), ((type*)(_stack)->curr));              \
})

#define stack_back(_stack, type) ({                                           \
    ((_stack)->curr==(_stack)->top)                                          ?\
    (NULL)                                                                   :\
    ((type*)((_stack)->curr - sizeof(type)));                                 \
})

#endif