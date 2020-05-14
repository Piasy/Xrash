#include "crash.h"

#include <cstdlib>

NO_INLINE static int* newPtr() {
    return new int[1];
}

NO_INLINE static void resetPtr(int** ptr) {
    *ptr = nullptr;
}

NO_INLINE static void usePtr(int* ptr) {
    int var = *ptr;
    var++;
    *ptr = var;
}

void testCrash() {
    int* ptr = newPtr();
    usePtr(ptr);
    resetPtr(&ptr);
    usePtr(ptr);
    resetPtr(nullptr);
    usePtr(ptr);
}
