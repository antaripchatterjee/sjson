#include "sjson/@error.h"

#include <stdio.h>
#include <stdarg.h>

int sjson__build_error_message(char* error_message, size_t pos_index, const char* fmt, ...) {
    if (!error_message || !fmt) return 0;
    int written = 0;
    written = snprintf(error_message, SJSON__ERROR_MSG_SIZE, "Syntax Error at [%zu]. ", pos_index);
    if (written < 0) return 0;
    va_list args;
    va_start(args, fmt);
    int fmt_written = vsnprintf(error_message + written, SJSON__ERROR_MSG_SIZE - written, fmt, args);
    va_end(args);
    if (fmt_written < 0) return written;
    return written + fmt_written;
}