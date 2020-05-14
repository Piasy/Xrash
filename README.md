# Crash Symbolicate

C++ crash symbolicate for Android, iOS, Windows, etc.

## Android

Use Gradle + CMake to build C++ sources, and save the not stripped `.so` libs, which are located inside `build/intermediates/cmake/release/obj`. Check the `build_android.sh` for details. Note that you can build your sources with any NDK version.

When crash happens, collect it from logcat like below:

```bash
2020-05-14 11:46:30.526 17540-17540/? A/DEBUG: *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
2020-05-14 11:46:30.526 17540-17540/? A/DEBUG: Build fingerprint: 'google/walleye/walleye:9/PPR2.181005.003/4984323:user/release-keys'
2020-05-14 11:46:30.526 17540-17540/? A/DEBUG: Revision: 'MP1'
2020-05-14 11:46:30.526 17540-17540/? A/DEBUG: ABI: 'arm64'
2020-05-14 11:46:30.526 17540-17540/? A/DEBUG: pid: 17516, tid: 17516, name: y.crash.example  >>> com.piasy.crash.example <<<
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG: signal 5 (SIGTRAP), code 1 (TRAP_BRKPT), fault addr 0x6fe47f464c
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x0  0000006ffaee0460  x1  0000007fd4e64f14  x2  0000000000000000  x3  0000006ffae14c00
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x4  0000007fd4e65330  x5  0000006fe4cbac3e  x6  fefefefefefeff06  x7  7f7f7f7f7f7f7f7f
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x8  3dfaa4fa65dcd80b  x9  3dfaa4fa65dcd80b  x10 0000000000430000  x11 0000006ffadfa688
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x12 0000007080255d30  x13 d6bcd89b105e3297  x14 0000006ff1000000  x15 0000000000000000
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x16 0000006fe47f5ff0  x17 0000006fe47f464c  x18 00000000015cdb34  x19 0000006ffae14c00
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x20 0000006ffad13c00  x21 0000006ffae14c00  x22 0000007fd4e651a0  x23 0000006fe4cbac3e
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x24 0000000000000000  x25 00000070809015e0  x26 0000006ffae14ca0  x27 0000000000000000
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     x28 0000000000000000  x29 0000007fd4e64fc8
2020-05-14 11:46:30.527 17540-17540/? A/DEBUG:     sp  0000007fd4e64f00  lr  0000006fe4c4e0b4  pc  0000006fe47f464c
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG: backtrace:
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG:     #00 pc 000000000000064c  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/lib/arm64/libcrash.so (testCrash())
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG:     #01 pc 00000000000040b0  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/oat/arm64/base.odex (offset 0x4000) (com.piasy.crash.example.MainActivity.testCrash+144)
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG:     #02 pc 000000000055cc4c  /system/lib64/libart.so (art_quick_invoke_static_stub+604)
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG:     #03 pc 00000000000cf760  /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
2020-05-14 11:46:30.618 17540-17540/? A/DEBUG:     #04 pc 00000000002823b0  /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
```

Then use `ndk-stack` in NDK r21 to symbolicate it, note that only `ndk-stack` need to be NDK r21, because older version's `ndk-stack` can't symblicate arm64 crash, you can build your sources with older NDK.

```bash
ndk-stack -sym libs/android-unstripped/arm64-v8a -dump crash.txt
```

Choose the corresponding architecture of the crash, which is `arm64` in this case, as shown by `ABI: 'arm64'`.

The symbolicated stack should be:

```bash
********** Crash dump: **********
Build fingerprint: 'google/walleye/walleye:9/PPR2.181005.003/4984323:user/release-keys'
#00 0x000000000000064c /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/lib/arm64/libcrash.so (testCrash())
                                                                                                         usePtr(int*)
                                                                                                         /Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:16:10
                                                                                                         testCrash()
                                                                                                         /Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:23:0
#01 0x00000000000040b0 /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/oat/arm64/base.odex (offset 0x4000) (com.piasy.crash.example.MainActivity.testCrash+144)
#02 0x000000000055cc4c /system/lib64/libart.so (art_quick_invoke_static_stub+604)
#03 0x00000000000cf760 /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
#04 0x00000000002823b0 /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
```

We can see the file name and line number now.

If the crash report is collected by some crash report system, e.g. Bugly, then we can get stacktrace like this:

```bash
1 #00 pc 000000000000064c  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/lib/arm64/libcrash.so (testCrash())
2 #01 pc 00000000000040b0  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/oat/arm64/base.odex (offset 0x4000) (com.piasy.crash.example.MainActivity.testCrash+144)
3 #02 pc 000000000055cc4c  /system/lib64/libart.so (art_quick_invoke_static_stub+604)
4 #03 pc 00000000000cf760  /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
5 #04 pc 00000000002823b0  /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
```

We can just insert two lines before it, make it looks like this:

```bash
*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
pid: 0, tid: 0
1 #00 pc 000000000000064c  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/lib/arm64/libcrash.so (testCrash())
2 #01 pc 00000000000040b0  /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/oat/arm64/base.odex (offset 0x4000) (com.piasy.crash.example.MainActivity.testCrash+144)
3 #02 pc 000000000055cc4c  /system/lib64/libart.so (art_quick_invoke_static_stub+604)
4 #03 pc 00000000000cf760  /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
5 #04 pc 00000000002823b0  /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
```

Then it could be symbolicated with the same command we used earlier, the result should be:

```bash
********** Crash dump: **********
#00 0x000000000000064c /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/lib/arm64/libcrash.so (testCrash())
                                                                                                         usePtr(int*)
                                                                                                         /Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:16:10
                                                                                                         testCrash()
                                                                                                         /Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:23:0
#01 0x00000000000040b0 /data/app/com.piasy.crash.example-WwX794UZhqOGrCcsXYAMjA==/oat/arm64/base.odex (offset 0x4000) (com.piasy.crash.example.MainActivity.testCrash+144)
#02 0x000000000055cc4c /system/lib64/libart.so (art_quick_invoke_static_stub+604)
#03 0x00000000000cf760 /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
#04 0x00000000002823b0 /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
```

## iOS

Use CMake + `xcodebuild` to build sources, and save `dSYM`, check the `build_libs_ios.sh` for details.

When crash happens, collect it from "View Device Logs", save it to `1.crash`, and put all `dSYM` with it in the same dir, then symbolicate with:

```bash
# cd into the dir containing 1.crash and all .dSYM
/Applications/Xcode.app/Contents/SharedFrameworks/DVTFoundation.framework/Versions/Current/Resources/symbolicatecrash 1.crash
```

The un-symbolicated stack should be (only show several frames):

```bash
Thread 0 name:  Dispatch queue: com.apple.main-thread
Thread 0 Crashed:
0   crash                         	0x0000000104767f94 0x104760000 + 32660
1   CrashExample                  	0x000000010474e4b0 0x104748000 + 25776
2   UIKitCore                     	0x00000001b3c4e920 -[UIViewController _sendViewDidLoadWithAppearanceProxyObjectTaggingEnabled] + 100
3   UIKitCore                     	0x00000001b3c53430 -[UIViewController loadViewIfRequired] + 936
4   UIKitCore                     	0x00000001b3c53838 -[UIViewController view] + 28
```

The symbolicated stack should be:

```bash
Thread 0 name:  Dispatch queue: com.apple.main-thread
Thread 0 Crashed:
0   crash                         	0x0000000104767f94 testCrash() + 32660 (crash.cpp:23)
1   CrashExample                  	0x000000010474e4b0 0x104748000 + 25776
2   UIKitCore                     	0x00000001b3c4e920 -[UIViewController _sendViewDidLoadWithAppearanceProxyObjectTaggingEnabled] + 100
3   UIKitCore                     	0x00000001b3c53430 -[UIViewController loadViewIfRequired] + 936
4   UIKitCore                     	0x00000001b3c53838 -[UIViewController view] + 28
```

We can see the file name and line number now.

If the crash report is collected by some crash report system, e.g. Bugly, then we can get stacktrace like this:

```bash
0   crash                         	0x0000000104767f94 0x104760000 + 32660
1   CrashExample                  	0x000000010474e4b0 0x104748000 + 25776
2   UIKitCore                     	0x00000001b3c4e920 -[UIViewController _sendViewDidLoadWithAppearanceProxyObjectTaggingEnabled] + 100
3   UIKitCore                     	0x00000001b3c53430 -[UIViewController loadViewIfRequired] + 936
4   UIKitCore                     	0x00000001b3c53838 -[UIViewController view] + 28
5   UIKitCore                     	0x00000001b42a4690 -[UIWindow addRootViewControllerViewIfPossible] + 172
```

We can use `xcrun` to symbolicate each line manually, but I write a script for it.

Save the stacktrace to `2.crash`, and put all `dSYM` with it in the same dir, then symbolicate with:

```bash
# cd into the dir containing 2.crash and all .dSYM
python3 sybolicatereport.py 2.crash arm64
```

Choose the corresponding architecture of the crash, which is arm64 in this case.

The result should be:

```bash
0 crash 0x0000000104767f94 testCrash() (in crash) (crash.cpp:23)
1 CrashExample 0x000000010474e4b0 0x104748000 + 25776
2 UIKitCore 0x00000001b3c4e920 -[UIViewController _sendViewDidLoadWithAppearanceProxyObjectTaggingEnabled] + 100
3 UIKitCore 0x00000001b3c53430 -[UIViewController loadViewIfRequired] + 936
4 UIKitCore 0x00000001b3c53838 -[UIViewController view] + 28
5 UIKitCore 0x00000001b42a4690 -[UIWindow addRootViewControllerViewIfPossible] + 172
```

## Windows

TBD.
