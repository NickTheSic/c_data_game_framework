@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw3dll
set GLAD=../vendor/src/glad.c

set FLAGS= -g -Wall
@echo on

clang++ %FLAGS% %INCLUDES% %LIBS% %GLAD% ../src/main.cpp -o a.exe

@echo off
popd 
@echo on
