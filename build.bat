@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set LIBS=-L../lib -lglfw3dll -lopengl32 -lgdi32 -luser32
set FLAGS= -g -Wall -Wextra -DGLFW_PLATFORM_LAYER
set EXTRA= %1%

@echo on

clang++ %FLAGS% %INCLUDES% %LIBS% %EXTRA% ../src/build.cpp -o a.exe

@echo off
popd 
@echo on
