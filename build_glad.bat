
set INCLUDES=-Ivendor/include
set SRC=vendor/src/glad.c
set FLAGS= -g -Wall -Wextra -c

clang %INCLUDES% %SRC% %FLAGS% -fuse-ld=llvm-lib -o lib/glad.lib

