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
    size_t _hash = HASH_BASE;               \
    while(*_x) {                            \
        _hash=((_hash << 5) + _hash)+*_x++; \
    }                                       \
    _hash;                                  \
})
#define hash_n(x,n) ({                      \
    size_t _hash = HASH_BASE;                \
    for (int i_ = 0 ; i_ < (int)n ; ++i_)      \
        _hash=((_hash << 5) + _hash) + x[i_];\
    _hash;                                  \
})

#endif