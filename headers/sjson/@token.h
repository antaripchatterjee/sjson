#ifndef __SJSON_TOKEN_H__
#define __SJSON_TOKEN_H__

#include "../common.h"
#include "sjson/@context.h"

#include <stddef.h>
#include <stdbool.h>

typedef enum sjson__tokentype_t {
    SJSON__T_TOKEN_INVALID = -1,
    SJSON__T_TOKEN_UNKNOWN,
    SJSON__T_OPERAND_STRING_VALUE,
    SJSON__T_OPERAND_DECIMAL_INTEGER,
    SJSON__T_OPERAND_DECIMAL_DOUBLE,
    SJSON__T_SYMBOL_SIMPLE_COMMA,
    SJSON__T_SYMBOL_SIMPLE_COLON,
    SJSON__T_SYMBOL_SIMPLE_SQRBRO,
    SJSON__T_SYMBOL_SIMPLE_SQRBRC,
    SJSON__T_SYMBOL_SIMPLE_CURLBRO,
    SJSON__T_SYMBOL_SIMPLE_CURLBRC,
    SJSON__T_RESERVED_KEYWORD_CONSTANT_FALSE,
    SJSON__T_RESERVED_KEYWORD_CONSTANT_NULL,
    SJSON__T_RESERVED_KEYWORD_CONSTANT_TRUE
}; // enum sjson__tokentype_t

struct sjson__token_t {
    enum sjson__tokentype_t token_type;
    char* token_buffer;
    size_t pos;
    struct sjson__token_t* next_token;
}; // struct sjson__token_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DLLEXPORT
bool sjson__is_boolean_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_null_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_string_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_number_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_value_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_array_start_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_array_end_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_object_start_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_object_end_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_comma_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);

DLLEXPORT
bool sjson__is_colon_token(enum sjson__tokentype_t token_type, struct sjson__context_t *context);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_TOKEN_H__
