@echo off
if not exist build mkdir build
pushd build

set INCLUDES=-I../include -I../vendor/include
set SRC_INCLUDES=-I../src
set DEFINES=-DPLATFORM_WEB
set LIBS=-L../lib -lopenal
set FLAGS= -Wall -Wextra -Werror -O2 -s ALLOW_MEMORY_GROWTH=1 -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -sFULL_ES3 -sFULL_ES2
set DATA=--preload-file data 
set SHELL_FILE=--shell-file ../MinimalShell.html
set EXTRA= %1 %2 %3 

@echo on

em++ %FLAGS% %INCLUDES% %SRC_INCLUDES% %LIBS% %EXTRA% ../src/build.cpp -o index.html %DATA%

@echo off
popd 
@echo on
