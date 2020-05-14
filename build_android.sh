#!/bin/bash

UNSTRIPPED_SRC=Android/build/intermediates/cmake/release/obj
UNSTRIPPED_DIST=libs/android-unstripped
SO_LIB=libcrash.so
ARCH=( "armeabi-v7a" "arm64-v8a" "x86" "x86_64" )

function cp_unstripped_so_libs() {
  for arc in "${ARCH[@]}"
  do
    mkdir -p ${UNSTRIPPED_DIST}/${arc}
    cp ${UNSTRIPPED_SRC}/${arc}/${SO_LIB} ${UNSTRIPPED_DIST}/${arc}/
  done
}

rm -rf ${UNSTRIPPED_DIST}

./gradlew clean :Android:assembleRelease && \
cp_unstripped_so_libs
