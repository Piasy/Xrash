#pragma once

#ifdef _WIN32
    #ifdef CRASH_EXPORTS
        #define CRASH_API __declspec(dllexport)
    #else
        #define CRASH_API __declspec(dllimport)
    #endif
    
    #define NO_INLINE __declspec(noinline)
#else
    #define CRASH_API

    #define NO_INLINE __attribute__ ((noinline))
#endif
