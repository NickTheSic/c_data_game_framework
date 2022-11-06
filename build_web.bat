@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw
set FLAGS= -Wall -Wextra -o2 -sUSE_GLFW
set DATA=--preload-file data

@echo on

em++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
