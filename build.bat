@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw3dll -lglad
set GLAD=../vendor/src/glad.c

set FLAGS= -g -Wall -Wextra -Werror
@echo on

clang++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
