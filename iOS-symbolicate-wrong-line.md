# iOS symbolicate wrong line number

I consult this problem to Apple DTS engineer, I want to share replies at here.

_Please note that I'm using a slightly different example in the consulting, so the line number is a little bit different_.

## First reply

Consider the following examples from your sample. Here’s a backtrace from LLDB:

```bash
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x0)
  * frame #0: 0x000000010457a30c CrashExample`usePtr(ptr=0x0000000000000000) at crash.cpp:14:15 [opt]
    frame #1: 0x000000010457a34c CrashExample`testCrash() at crash.cpp:23:5 [opt]
    frame #2: 0x000000010457a480 CrashExample`-[ViewController viewDidLoad](self=<unavailable>, _cmd=<unavailable>) at ViewController.mm:22:5 [opt]
```

And here are the frames from the crash report if I detach LLDB for the same process:

```bash
0   CrashExample                  	0x000000010457a30c usePtr(int*) (in CrashExample) (crash.cpp:13)
1   CrashExample                  	0x000000010457a34c testCrash() (in CrashExample) (crash.cpp:24)
2   CrashExample                  	0x000000010457a480 -[ViewController viewDidLoad] (in CrashExample) (ViewController.mm:23)
```

Notice the instruction addresses are the same. There’s an important detail to what these addresses mean: they are the return addresses, or the instruction that will execute next when returning to that stack frame (with the exception of frame zero, which points to the current instruction). They don’t represent the instruction that called into other stack frames.

## Follow up

Since LLDB could show the correct line number, so I asked if it's possible to use the same symbolicate process as LLDB.

The reply is:

I spent time talking to the LLDB team to get more info about this, such as any other knowledge about why the program counter is adjusted is some circumstances, and not in others, that I could pass along.

For most app circumstances, they agree that what you’re seeing in your examples is confusing. However, uniformly adjusting the program counter in all scenarios isn’t possible from the debugger’s point of view at this time. There are important scenarios in the system that intersect the state of a crashed process, namely signal handlers, where doing such an adjustment would distort the crashed process state and the unmodified program counter value needs to be preserved for the crash report. As such, there isn’t a way to get a symbolicated crash report from the tools that exactly matches the backtrace recorded by LLDB.

## Conclusion

Current toolchain can't help with this problem, but we can write shorter functions, so that we can deduce the possible line number by analyze code statically.
