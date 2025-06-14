#ifndef __SJSON_HELPER_H__
#define __SJSON_HELPER_H__

#include "../common.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DLLEXPORT
int sjson__build_error_message(char* error_message, size_t pos_index, const char* fmt, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_HELPER_H__
