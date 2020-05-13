#include <jni.h>

#include "crash.h"

extern "C" {

JNIEXPORT void JNICALL
Java_com_piasy_crash_MainActivity_testCrash(
        JNIEnv*, jclass) {
    testCrash();
}

}
