@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw
set FLAGS= -Wall -Wextra -o2

@echo on

em++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
