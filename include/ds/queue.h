#ifndef __QUEUE_H
#define __QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

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
    ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))         ?\
      ((_queue)->front = (_queue)->back = (_queue)->base, NULL)            :\
      ((_queue)->front+sizeof(type) > ((_queue)->base+(_queue)->length)) ?  \
      (NULL)                                                             :  \
      ((_queue)->front+=sizeof(type), (type*)__current);                    \
})

#define queue_front(_queue, type) ({                                  \
    ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))   ?\
    (NULL)                                                           :\
    ((_queue)->front);                                                \
})

#define queue_back(_queue, type) ({                                   \
    ((uintptr_t)((_queue)->front - (_queue)->back) < sizeof(type))   ?\
    (NULL)                                                           :\
    ((_queue)->back);                                                 \
})

#endif