@echo off
setlocal enableextensions enabledelayedexpansion

rd /S /Q build\Windows
mkdir build\Windows
mkdir symbols\win_x86

cmake -S . -B build\Windows && ^
cmake --build build\Windows --config Release --target crash && ^
copy /Y build\Windows\Release\crash.dll libs\ && ^
copy /Y build\Windows\Release\crash.lib libs\ && ^
copy /Y build\Windows\Release\crash.pdb symbols\win_x86
