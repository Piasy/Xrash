#!/bin/bash

UNSTRIPPED_SRC=Android/build/intermediates/cmake/release/obj
UNSTRIPPED_DEST=symbols
SO_LIB=libcrash.so
ARCH=( "armeabi-v7a" "arm64-v8a" "x86" "x86_64" )

function cp_unstripped_so_libs() {
  for arc in "${ARCH[@]}"
  do
    mkdir -p ${UNSTRIPPED_DEST}/${arc}
    cp ${UNSTRIPPED_SRC}/${arc}/${SO_LIB} ${UNSTRIPPED_DEST}/${arc}/
  done
}

./gradlew clean :Android:assembleRelease && \
cp_unstripped_so_libs
