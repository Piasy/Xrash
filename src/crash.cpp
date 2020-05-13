#include "crash.h"

#include <cstdlib>

static int* newPtr() {
    return new int[1];
}

static void resetPtr(int** ptr) {
    *ptr = nullptr;
}

static void usePtr(int* ptr) {
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
