#!/bin/bash

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/libs

cd LinuxExample
./CrashExample
