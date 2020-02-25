#ifndef __BASIC_HASH_H
#define __BASIC_HASH_H

#ifdef DEBUG
#include <stdio.h>
#endif

#ifndef HASH_BASE
#define HASH_BASE 5381
#endif 

#define hash(x) ({                          \
    const char* _x = x;                     \
    int _hash = HASH_BASE;                  \
    while(*_x) {                            \
        printf("%c\n", *_x);                \
        _hash=((_hash << 5) + _hash)+*_x++; \
    }                                       \
    _hash;                                  \
})
#define hash_n(x,n) ({                      \
    int _hash = HASH_BASE;                  \
    for (int i = 0 ; i < (int)n ; ++i)      \
        _hash=((_hash << 5) + _hash) + x[i];\
    _hash;                                  \
})

#endif