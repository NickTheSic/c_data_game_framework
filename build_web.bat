@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../include/core -I../include/game -I../include/input -I../include/renderer -I../include/utils -I../include/ui -I../vendor/include
set SRC_INCLUDES=-I../src -I../src/core -I../src/game -I../src/input -I../src/renderer -I../src/ui
set DEFINES=-DPLATFORM_WEB
set LIBS=-L../lib
set FLAGS= -Wall -Wextra -o2
set DATA=--preload-file data

@echo on

em++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe %DATA%

@echo off
popd 
@echo on
