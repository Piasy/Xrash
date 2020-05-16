# Crash Symbolicate

C++ crash symbolicate for Android, iOS, Windows, etc.

## Android

Use Gradle + CMake to build C++ sources, and save the unstripped `.so` libs, which are located inside `build/intermediates/cmake/release/obj`. Check the `build_android.sh` for details. Note that you can build your sources with any NDK version.

When crash happens, collect it from logcat like below:

```bash
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: Build fingerprint: 'google/bullhead/bullhead:8.1.0/OPM6.171019.030.B1/4768815:user/release-keys'
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: Revision: 'rev_1.0'
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: ABI: 'arm64'
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: pid: 10229, tid: 10229, name: y.crash.example  >>> com.piasy.crash.example <<<
2020-05-16 09:25:36.508 10259-10259/? A/DEBUG: signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG: Cause: null pointer dereference
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x0   0000000000000000  x1   0000000000000000  x2   0000000000000000  x3   0000007976ebea00
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x4   0000007fd259c890  x5   000000796c088c32  x6   0000000000000000  x7   7f7f7f7f7f7f7f7f
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x8   0000000000585859  x9   42af910898b7fabb  x10  0000000000000015  x11  00000000ffffffff
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x12  0000007976e0d5e0  x13  5b9e86d03edc445b  x14  0000000000000000  x15  0000007960400000
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x16  00000079fb22cca8  x17  00000079fb1c94b8  x18  000000796be6a820  x19  0000000000000000
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x20  0000007976cf0d90  x21  0000007976ebea00  x22  0000007fd259c70c  x23  000000796c088c32
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x24  0000000000000000  x25  00000079fbb38a40  x26  0000007976ebeaa0  x27  0000000000000000
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     x28  0000000000000000  x29  0000007fd259c460  x30  000000796098b6c8
2020-05-16 09:25:36.509 10259-10259/? A/DEBUG:     sp   0000007fd259c450  pc   000000796098b6e8  pstate 0000000060000000
2020-05-16 09:25:36.513 10259-10259/? A/DEBUG: backtrace:
2020-05-16 09:25:36.513 10259-10259/? A/DEBUG:     #00 pc 00000000000006e8  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so
2020-05-16 09:25:36.513 10259-10259/? A/DEBUG:     #01 pc 00000000000006c4  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so (testCrash()+40)
2020-05-16 09:25:36.513 10259-10259/? A/DEBUG:     #02 pc 00000000000040b0  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/oat/arm64/base.odex (offset 0x4000)
```

Then use `ndk-stack` in NDK r21 to symbolicate it, note that only `ndk-stack` need to be NDK r21, because older version's `ndk-stack` can't symblicate arm64 crash, you can build your sources with older NDK.

```bash
ndk-stack -sym libs/android-unstripped/arm64-v8a -dump crash.txt
```

Choose the corresponding architecture of the crash, which is `arm64` in this case, as shown by `ABI: 'arm64'`.

The symbolicated stack should be:

```bash
********** Crash dump: **********
Build fingerprint: 'google/bullhead/bullhead:8.1.0/OPM6.171019.030.B1/4768815:user/release-keys'
#00 0x00000000000006e8 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so
usePtr(int*)
/Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:14:15
#01 0x00000000000006c4 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so (testCrash()+40)
testCrash()
/Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:23:5
#02 0x00000000000040b0 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/oat/arm64/base.odex (offset 0x4000)
```

We can see the file name and line number now.

If the crash report is collected by some crash report system, e.g. Bugly, then we can get stacktrace like this:

```bash
1 #00 pc 00000000000006e8  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so
2 #01 pc 00000000000006c4  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so (testCrash()+40)
3 #02 pc 00000000000040b0  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/oat/arm64/base.odex (offset 0x4000)
```

We can just insert two lines before it, make it looks like this:

```bash
*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
pid: 0, tid: 0
1 #00 pc 00000000000006e8  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so
2 #01 pc 00000000000006c4  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so (testCrash()+40)
3 #02 pc 00000000000040b0  /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/oat/arm64/base.odex (offset 0x4000)
```

Then it could be symbolicated with the same command we used earlier, the result should be:

```bash
********** Crash dump: **********
#00 0x00000000000006e8 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so
usePtr(int*)
/Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:14:15
#01 0x00000000000006c4 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/lib/arm64/libcrash.so (testCrash()+40)
testCrash()
/Users/piasy/src/CrashSymbolicate/Android/.externalNativeBuild/cmake/release/arm64-v8a/../../../../../src/crash.cpp:23:5
#02 0x00000000000040b0 /data/app/com.piasy.crash.example-na2ugYuDMIIBetyZjBi90A==/oat/arm64/base.odex (offset 0x4000)
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
0   crash                         	0x000000010009ff54 0x100098000 + 32596
1   crash                         	0x000000010009ff30 0x100098000 + 32560
2   CrashExample                  	0x00000001000864b0 0x100080000 + 25776
3   UIKitCore                     	0x000000018ea0a36c 0x18e60d000 + 4182892
4   UIKitCore                     	0x000000018ea0ef20 0x18e60d000 + 4202272
```

The symbolicated stack should be:

```bash
Thread 0 name:  Dispatch queue: com.apple.main-thread
Thread 0 Crashed:
0   crash                         	0x000000010009ff54 usePtr(int*) + 32596 (crash.cpp:13)
1   crash                         	0x000000010009ff30 testCrash() + 32560 (crash.cpp:24)
2   CrashExample                  	0x00000001000864b0 0x100080000 + 25776
3   UIKitCore                     	0x000000018ea0a36c -[UIViewController _sendViewDidLoadWithAppearanceProxyObjectTaggingEnabled] + 104
4   UIKitCore                     	0x000000018ea0ef20 -[UIViewController loadViewIfRequired] + 952
```

We can see the file name and line number now.

**NOTE**: the symbolicate result is not correct, the first frame should be line 14, the second frame should be line 23, I need to investigate it further.

If the crash report is collected by some crash report system, e.g. Bugly, then we can get stacktrace like this:

```bash
0   crash                         	0x0000000100357f54 0x100350000 + 32596
1   crash                         	0x0000000100357f30 0x100350000 + 32560
2   CrashExample                  	0x000000010033e4b0 0x100338000 + 25776
3   UIKitCore                     	0x000000018ea0a36c 0x18e60d000 + 4182892
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
0 crash 0x0000000100357f54 usePtr(int*) (in crash) (crash.cpp:13)
1 crash 0x0000000100357f30 testCrash() (in crash) (crash.cpp:24)
2 CrashExample 0x000000010033e4b0 0x100338000 + 25776
3 UIKitCore 0x000000018ea0a36c 0x18e60d000 + 4182892
```

## Windows

TBD.
