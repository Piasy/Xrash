# Xrash

Xrash is a CMake based cross-platform project for C++ sources, and also illustrate crash symbolicate within them.

Xrash supports Android, iOS, Windows, and Linux now.

_Note, for iOS and Windows, CMake 3.17.2 is suggested_.

## Android

Use Gradle + CMake to build C++ sources, and save the unstripped `.so` libs, which are located inside `build/intermediates/cmake/release/obj`. Check `build_android.sh` for details. Note that you can build your sources with any NDK version.

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

Use CMake + `xcodebuild` to build sources, and save `dSYM`, check `build_libs_ios.sh` for details.

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

We can use `xcrun` to symbolicate each line manually, but I write a Python script for it.

Save the stacktrace to `2.crash`, and put all `dSYM` with it in the same dir, then symbolicate with:

```bash
# cd into the dir containing 2.crash and all .dSYM
python3 symbolicatereport.py 2.crash arm64
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

Use CMake to build sources, ~~and save `.pdb` file~~, check `build_lib_windows.bat` for details. Use `MiniDumpWriteDump` from `DbgHelp` to write a minudump when crash happens, check `Win32Example/Win32Example.cpp` for details.

`windbg` could analyze minidump with `!analyze -v` command, but it could only get the line number of the last frame, which isn't very helpful. After some googling, I didn't find how to get line number of all frames, so I decide to switch to `minidump_stackwalk`. To use `minidump_stackwalk`, we need convert `.pdb` to `.sym`, and save `.sym` file, check `build_lib_windows.bat` for details.

I also create a Python script to help me calling `minidump_stackwalk` on macOS, note we can use `minidump_stackwalk` on macOS or Linux, we only need to convert `.pdb` to `.sym` on Windows.

Put the `.dmp` and `.sym` in the same dir, then symbolicate with:

```bash
# cd into the dir containing .dmp and .sym
python3 walk_stack.py <.dmp file>
# if the output is too long, we can redirect all the output into files with
# > 1.crash 2>&1
```

Note that `walk_stack.py` and `minidump_stackwalk` should be put in the same dir.

The result should be (only show several frames):

```bash
Thread 0 (crashed)
 0  crash.dll!static void usePtr(int *) [crash.cpp : 16 + 0x3]
    eip = 0x708c1066   esp = 0x0136e910   ebp = 0x0136e910   ebx = 0x00e716a0
    esi = 0x00c10612   edi = 0x00000111   eax = 0x00000000   ecx = 0x00000000
    edx = 0x00000000   efl = 0x00010202
    Found by: given as instruction pointer in context
 1  crash.dll!testCrash() [crash.cpp : 23 + 0x9]
    eip = 0x708c1044   esp = 0x0136e918   ebp = 0x0136e928
    Found by: call frame info
 2  Win32Example.exe + 0x1792
    eip = 0x00e71792   esp = 0x0136e930   ebp = 0x0136e984
    Found by: call frame info
 3  user32.dll + 0x45cab
    eip = 0x76d75cab   esp = 0x0136e98c   ebp = 0x0136e9b0
    Found by: previous frame's frame pointer
```

We can see the file name and line number now, we can ignore the `+ 0x` after the line number.

## Linux

Use CMake and make to build sources, add `-g` option in `CMakeLists.txt`, dump symbols using `dump_syms` from [breakpad](https://chromium.googlesource.com/breakpad/breakpad/), and save `.sym` file, check `build_lib_linux.sh` for details.

Use `google_breakpad::ExceptionHandler` to catch crash and write minidump, check `LinuxExample/CrashExample.cpp` for details.

When crash happens, we can symbolicate it in the same way as Windows.

Put the `.dmp` and `.sym` in the same dir, then symbolicate with:

```bash
# cd into the dir containing .dmp and .sym
python3 walk_stack.py <.dmp file>
# if the output is too long, we can redirect all the output into files with
# > 1.crash 2>&1
```

The result should be (only show several frames):

```bash
Thread 0 (crashed)
 0  libcrash.so!usePtr [crash.cpp : 14 + 0x4]
    rax = 0x0000000000000000   rdx = 0x0000000000000001
    rcx = 0x00005592e78a1100   rbx = 0x0000000000000000
    rsi = 0x0000000000000000   rdi = 0x0000000000000000
    rbp = 0x00007ffd1c4c91b0   rsp = 0x00007ffd1c4c91b0
     r8 = 0x0000000000000000    r9 = 0x0000000000000000
    r10 = 0x00005592e788b010   r11 = 0x0000000000000000
    r12 = 0x00005592e66d5360   r13 = 0x00007ffd1c4c9460
    r14 = 0x0000000000000000   r15 = 0x0000000000000000
    rip = 0x00007f8d674c56fc
    Found by: given as instruction pointer in context
 1  libcrash.so!testCrash() [crash.cpp : 23 + 0xc]
    rbx = 0x0000000000000000   rbp = 0x00007ffd1c4c91d0
    rsp = 0x00007ffd1c4c91c0   r12 = 0x00005592e66d5360
    r13 = 0x00007ffd1c4c9460   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x00007f8d674c5755
    Found by: call frame info
 2  CrashExample + 0x2560
    rbx = 0x0000000000000000   rbp = 0x00007ffd1c4c9380
    rsp = 0x00007ffd1c4c91e0   r12 = 0x00005592e66d5360
    r13 = 0x00007ffd1c4c9460   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x00005592e66d5560
    Found by: call frame info
 3  CrashExample + 0x246a
    rbp = 0x00007ffd1c4c9380   rsp = 0x00007ffd1c4c9290
    rip = 0x00005592e66d546a
    Found by: stack scanning
```

We can see the file name and line number now, we can ignore the `+ 0x` after the line number.
