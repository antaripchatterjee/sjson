#ifndef __SJSON_CONTEXT_H__
#define __SJSON_CONTEXT_H__

#include "../common.h"

#include <stddef.h>

struct sjson__context_t {
    size_t object_depth;
    size_t array_depth;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_CONTEXT_H__
