@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw3dll -lglad
set FLAGS= -g -Wall -Wextra 

@echo on

clang++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
