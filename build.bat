@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../include/game -I../include/input -I../include/utils -I../vendor/include
set SRC_INCLUDES=-I../src -I../src/game -I../src/input
set LIBS=-L../lib -lopengl32 -lgdi32 -luser32
set FLAGS= -g -Wall -Wextra
set EXTRA= %1 %2 %3 

@echo on

clang++ %FLAGS% %INCLUDES% %SRC_INCLUDES% %LIBS% %EXTRA% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
