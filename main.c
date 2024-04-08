#include <stdio.h>
#include "heap.h"
#include "tested_declarations.h"
#include "rdebug.h"
int main() {
    heap_setup();
    char *ptr = heap_calloc(100, 1);
//    ptr = ptr + 100;
    for (int i = 0; i < 100; ++i) {
        printf("%d ", *(ptr + i));
    }
    heap_clean();
    return 0;
}

