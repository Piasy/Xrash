#!/bin/bash

PROJECT=CrashSymbolication
TARGET=crash
DIST=libs
CONF=Release

THIN_FRAMEWORK_OS=build/iOS/${CONF}-iphoneos/${TARGET}.framework
THIN_DSYM_OS=build/iOS/${CONF}-iphoneos/${TARGET}.framework.dSYM
THIN_FRAMEWORK_SIM=build/iOS/${CONF}-iphonesimulator/${TARGET}.framework
THIN_DSYM_SIM=build/iOS/${CONF}-iphonesimulator/${TARGET}.framework.dSYM
FAT_FRAMEWORK=${DIST}/${TARGET}.framework
FAT_DSYM=${DIST}/${TARGET}.framework.dSYM

rm -rf build/iOS ${FAT_FRAMEWORK} ${FAT_DSYM}
mkdir -p build/iOS

# gen project and build
cmake -S . -B build/iOS -G Xcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    "-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.0 && \
xcodebuild -quiet \
    -project build/iOS/${PROJECT}.xcodeproj \
    -configuration ${CONF} \
    -sdk iphoneos \
    -target ${TARGET} && \
xcodebuild -quiet \
    -project build/iOS/${PROJECT}.xcodeproj \
    -configuration ${CONF} \
    -sdk iphonesimulator \
    -target ${TARGET} && \
# strip and codesign
strip -S ${THIN_FRAMEWORK_OS}/${TARGET} ${THIN_FRAMEWORK_SIM}/${TARGET} && \
/usr/bin/codesign --force --sign - --timestamp=none \
    ${THIN_FRAMEWORK_SIM}/${TARGET} && \
# dist framework
cp -r ${THIN_FRAMEWORK_OS} ${FAT_FRAMEWORK} && \
rm -f ${FAT_FRAMEWORK}/${TARGET} && \
lipo -create -output ${FAT_FRAMEWORK}/${TARGET} \
    ${THIN_FRAMEWORK_OS}/${TARGET} \
    ${THIN_FRAMEWORK_SIM}/${TARGET}
# dist dSYM
cp -r ${THIN_DSYM_OS} ${FAT_DSYM} && \
rm ${FAT_DSYM}/Contents/Resources/DWARF/${TARGET} && \
lipo -create -output \
    ${FAT_DSYM}/Contents/Resources/DWARF/${TARGET} \
    ${THIN_DSYM_OS}/Contents/Resources/DWARF/${TARGET} \
    ${THIN_DSYM_SIM}/Contents/Resources/DWARF/${TARGET}
