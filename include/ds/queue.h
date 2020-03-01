#ifndef __QUEUE_H
#define __QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#define dump_queue(stream, _queue, type, spec, args) ({     \
    const type* __front = (_queue)->front;                  \
    while (__front < (type*)((_queue)->back)) {                     \
        fprintf(stream, "dump_queue:%s:%d:"spec"\n", __FILE__, __LINE__, args);\
        __front++;                                   \
    }                                                       \
})
#else
#define dump_stack(...)
#endif

typedef struct queue {
    void* base;
    void* front;
    void* back;
    size_t length;
}queue_t;

#define queue_create(_queue, block, size) ({                \
    (_queue)->base = (_queue)->front = (((_queue)->back     \
    = block)+size)-size;                                    \
    (_queue)->length = size;              })

#define queue_push_back(_queue, element, type) ({                                    \
    const void* __current = (_queue)->back;                                          \
    ((_queue)->back+sizeof(type) > (_queue)->base + (_queue)->length)  ?             \
    (NULL)                                                 :                         \
    ((*(type*)(_queue)->back)=element,(_queue)->back += sizeof(type), __current);})  \

#define queue_pop_front(_queue, type) ({                                    \
    const void* __current = (_queue)->front;                                \
    if ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))       \
        (_queue)->front = (_queue)->back = (_queue)->base;                  \
    ((_queue)->front+sizeof(type) > (_queue)->back)   ?  \
    (NULL)                                                               :  \
    ((type*)((_queue)->front+=sizeof(type)), (type*)__current);             \
})

#define queue_front(_queue, type) ({                                  \
    ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))   ?\
    (NULL)                                                           :\
    ((type*)((_queue)->front));                                       \
})

#define queue_back(_queue, type) ({                                   \
    ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))   ?\
    (NULL)                                                           :\
    (((type*)((_queue)->back))-1);                                    \
})

#endif