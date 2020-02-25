#define DEBUG
#include "basic_hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* a = "143";

int main () {
    int b = 2;
    printf("%d %d\n", b=hash(a), hash_n(a, strlen(a)));
}