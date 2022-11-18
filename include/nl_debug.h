/* date = November 7th 2022 7:38 pm */

#ifndef NL_DEBUG_H
#define NL_DEBUG_H

#ifndef UNUSED
#define UNUSED(v) (void)(v);
#endif

#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(v) (void)(v);
#endif

#ifdef LOG
#warning LOG is already defined somewhere
#endif
#ifndef LOG
#define LOG(m, ...) fprintf(stdout, m,##__VA_ARGS__);
#endif

#endif //NL_DEBUG_H
