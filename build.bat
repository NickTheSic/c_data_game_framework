@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../include/audio -I../include/core -I../include/game -I../include/input -I../include/renderer -I../include/utils -I../include/ui -I../vendor/include
set SRC_INCLUDES=-I../src -I../src/core -I../src/game -I../src/input -I../src/renderer -I../src/ui
set DEFINES=-DPLATFORM_WINDOWS
set LIBS=-L../lib -lopengl32 -lgdi32 -luser32
set FLAGS= -g -Wall -Wextra
set EXTRA= %1 %2 %3 

@echo on

clang++ %FLAGS% %INCLUDES% %SRC_INCLUDES% %LIBS% %EXTRA% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
