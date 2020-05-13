cmake_minimum_required(VERSION 3.4.1)
project("CrashSymbolication")

file(GLOB crash_sources
        src/crash.cpp
        )

if (ANDROID)
        file(GLOB jni_sources
                src/crash_jni.cpp
                )

        add_library(crash SHARED
                ${crash_sources}
                ${jni_sources}
                )

        target_link_libraries(crash
                android
                )
elseif (MSVC)
endif ()
