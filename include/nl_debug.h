/* date = November 7th 2022 7:38 pm */

#include <stdio.h>
#include <assert.h>

#ifndef NL_DEBUG_H
#define NL_DEBUG_H

#ifndef UNUSED
 #define UNUSED(v) (void)(v);
#endif

#ifndef UNUSED_VARIABLE
 #define UNUSED_VARIABLE(v) (void)(v);
#endif

static const char* RED_CONSOLE_TEXT = "\33[31m";
static const char* WHITE_CONSOLE_TEXT = "\33[97m";
static const char* YELLOW_CONSOLE_TEXT = "\33[33m";
static const char* GREEN_CONSOLE_TEXT = "\33[32m";

#ifdef LOG
 #warning LOG is already defined somewhere
 #undef LOG
#endif
#ifndef LOG
 #ifdef __EMSCRIPTEN__
  #define LOG(m,...) fprintf(stdout, m,##__VA_ARGS__); fprintf(stdout, "\n");
 #else
  #define LOG(m, ...) fprintf(stdout, "%s %s line: %d -%s- \n", YELLOW_CONSOLE_TEXT, __FILE__, __LINE__, WHITE_CONSOLE_TEXT); \
                    fprintf(stdout, m,##__VA_ARGS__); fprintf(stdout, "\n");
 #endif
#endif

#endif //NL_DEBUG_H
