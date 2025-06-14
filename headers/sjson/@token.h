#ifndef __SJSON_TOKEN_H__
#define __SJSON_TOKEN_H__

#include "../common.h"
#include <stddef.h>

enum sjson__tokentype_t {
    SJSON__T_TOKEN_INVALID = -1,
    SJSON__T_TOKEN_UNKNOWN,
    SJSON__T_WHITESPACE_SPACE,
    SJSON__T_WHITESPACE_HORIZONTAL_TAB,
    SJSON__T_WHITESPACE_VERTICAL_TAB, 
    SJSON__T_WHITESPACE_FORM_FEED,
    SJSON__T_WHITESPACE_CARRIAGE_RETURN,
    SJSON__T_WHITESPACE_LINE_FEED,
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


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_TOKEN_H__
