@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib
set FLAGS= -Wall -Wextra -o2
set DATA=--preload-file data

@echo on

em++ %FLAGS% %INCLUDES% %LIBS% ../src/build.cpp -o a.exe %DATA%

@echo off
popd 
@echo on
