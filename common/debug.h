#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>
#include "log.h"

#ifdef __stensal__ // for error tracing purposes
#  define ABORT()    { char * p = 0; * p = 0; }
#else
#  define ABORT()    abort()
#endif

#define D_OUT stderr
#define D_FMT_PREFIX "[%s:%d] %s()\n\t"
#define D_FMT_ARGS __FILE__, __LINE__, __func__

// Encode a string with ANSI color
#ifdef LOG_USE_COLOR
# define ANSICOLOR(str, code) "\x1b["#code"m"str"\x1b[0m"
#else
# define ANSICOLOR(str, code) str
#endif

#define PUTS(msg) fprintf(D_OUT, D_FMT_PREFIX "%s\n", D_FMT_ARGS, msg)
#define NOTOP_PUTS(msg) fprintf(D_OUT, "\t%s\n", msg)
#define __PRINT(fmt, ...) fprintf(D_OUT, D_FMT_PREFIX fmt "\n%s", D_FMT_ARGS, __VA_ARGS__)
#define PRINT(...) __PRINT(__VA_ARGS__, "")
#define __NOTOP_PRINT(fmt, ...) fprintf(D_OUT, "\t" fmt "\n%s", __VA_ARGS__)
#define NOTOP_PRINT(...) __NOTOP_PRINT(__VA_ARGS__, "")
#define __ERR(fmt, ...) log_fatal(fmt"%s", __VA_ARGS__)
#define ERR(...)            \
  do {                      \
    __ERR(__VA_ARGS__, ""); \
    ABORT();                \
  } while (0)

#define ASSERT_S(expr, msg)                              \
  do {                                                   \
    if (!(expr)){                                        \
      ERR(ANSICOLOR("\n\tAssert Failed",31)":\t%s\n\t"   \
          ANSICOLOR("Expected",31)":\t" #expr, msg);     \
    }                                                    \
  } while(0)

// THIS WILL ONLY WORK IF __VA_ARGS__ IS SET
#define VASSERT_S(expr, fmt, ...)                            \
  do {                                                       \
    if (!(expr)){                                            \
      ERR(ANSICOLOR("\n\tAssert Failed",31)":\t"fmt"\n\t"    \
          ANSICOLOR("Expected",31)":\t" #expr, __VA_ARGS__); \
    }                                                        \
  } while(0)

#if _STATIC_DEBUG /* DEBUG MODE ACTIVE */

/* @param msg string to be printed in debug mode */
#  define D_PUTS(msg) PUTS(msg)
#  define D_NOTOP_PUTS(msg) NOTOP_PUTS(msg)
/* @param fmt like printf
   @param ... arguments to be parsed into fmt */
#  define D_PRINT(...) PRINT(__VA_ARGS__)
#  define D_NOTOP_PRINT(...) NOTOP_PRINT(__VA_ARGS__)
#  define D_ERR(...) ERR(__VA_ARGS__)
#  define D_ASSERT_S(expr, msg) ASSERT_S(expr, msg)
#  define D_ONLY(arg) (arg)

#else /* DEBUG MODE INNACTIVE */

#  define D_PUTS(msg) 
#  define D_NOTOP_PUTS(msg) 
#  define D_PRINT(...)
#  define D_NOTOP_PRINT(...)
#  define D_ERR(...)
#  define D_ASSERT_S(expr, msg)
#  define D_ONLY(arg)

#endif

#if _STRICT_STATIC_DEBUG /* DEBUG STRICT MODE ACTIVE */

/* @param msg string to be printed in debug mode */
#  define DS_PUTS(msg) PUTS(msg)
#  define DS_NOTOP_PUTS(msg) NOTOP_PUTS(msg)
/* @param fmt like printf
   @param ... arguments to be parsed into fmt */
#  define DS_PRINT(...) PRINT(__VA_ARGS__)
#  define DS_NOTOP_PRINT(...) NOTOP_PRINT(__VA_ARGS__)
#  define DS_ERR(...) ERR(__VA_ARGS__)
#  define DS_ASSERT_S(expr, msg) ASSERT_S(expr, msg)
#  define DS_ONLY(arg) (arg)

#else

#  define DS_PUTS(msg)
#  define DS_NOTOP_PUTS(msg)
#  define DS_PRINT(...)
#  define DS_NOTOP_PRINT(...)
#  define DS_ERR(...)
#  define DS_ASSERT_S(expr, msg)
#  define DS_ONLY(arg)

#endif

#endif // DEBUG_H
