#!/bin/bash

rm -rf build/Linux
mkdir -p symbols/linux_x64

mkdir -p build/Linux && cd build/Linux && \
cmake ../.. && \
make && \
../../tools/dump_syms_linux_x64 libcrash.so > \
    ../../symbols/linux_x64/libcrash.so.sym && \
strip -s libcrash.so && \
cp libcrash.so ../../libs && \
cp CrashExample ../../LinuxExample && \
cd ../../
