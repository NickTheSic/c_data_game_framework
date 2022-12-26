@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set SRC_INCLUDES=-I../src
set DEFINES=-DPLATFORM_WINDOWS
set LIBS=-L../lib -lopengl32 -lgdi32 -luser32
set FLAGS= -g -Wall -Wextra
set EXTRA= %1 %2 %3 

@echo on

clang++ %FLAGS% %INCLUDES% %SRC_INCLUDES% %LIBS% %EXTRA% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
