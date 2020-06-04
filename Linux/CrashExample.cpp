#include <client/linux/handler/exception_handler.h>
#include <crash.h>

#include <cstdio>

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                         void* context, bool succeeded) {
    printf("Dump path: %s\n", descriptor.path());
    return succeeded;
}

int main() {
    google_breakpad::MinidumpDescriptor descriptor(".");
    google_breakpad::ExceptionHandler eh(descriptor, nullptr, dumpCallback,
                                         nullptr, true, -1);

    testCrash();

    return 0;
}
