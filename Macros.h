#ifndef MACROS_H
#define MACROS_H

#include <iostream>

#define WRITE_TO_FILE(enable) write_to_file = enable;
#define INCLUDE_FILE(enable) include_file = enable;
#define INCLUDE_LINE(enable) include_line = enable;
#define INCLUDE_TOTAL_ALLOCS(enable) include_total_allocs = enable;
#define DEBUG_NEW new(__FILE__, include_line ? __LINE__ : 0)
#define new DEBUG_NEW

#endif // MACROS_H