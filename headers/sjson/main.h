#ifndef __SJSON_MAIN_H__
#define __SJSON_MAIN_H__

#include "../common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DLLEXPORT
int sjson__parse(const char* payload, char* error_message);

// DLLEXPORT
// int sjson__parse_from_file_pointer(FILE* fp, char* error_message);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_MAIN_H__
