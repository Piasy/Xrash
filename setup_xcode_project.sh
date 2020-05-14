#!/bin/bash

rm -rf iOS/CrashExample.xcodeproj && \
cd iOS && xcodegen && cd ..
