# actually we need 3.14+ to get first-class iOS support,
# but let's keep it lower than that for Android.
cmake_minimum_required(VERSION 3.4.0)
project("CrashSymbolication" C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

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
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
        file(GLOB objc_sources
                src/crash_test.mm
                )
        file(GLOB public_headers
                src/crash_test.h
                )

        add_library(crash SHARED
                ${crash_sources}
                ${objc_sources}
                ${public_headers}
                )

        set_target_properties(crash PROPERTIES
                FRAMEWORK TRUE
                MACOSX_FRAMEWORK_IDENTIFIER com.piasy.crash
                PUBLIC_HEADER "${public_headers}"
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
                XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
                XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO"
                XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES"
                XCODE_ATTRIBUTE_DYLIB_INSTALL_NAME_BASE "@rpath"
                INSTALL_RPATH "@executable_path/Frameworks;@loader_path/Frameworks"
                )
elseif (MSVC)
endif ()
