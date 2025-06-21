#ifndef __SJSON_GENTOK_H__
#define __SJSON_GENTOK_H__

#include "../common.h"
#include "sjson/@token.h"

#include <stddef.h>


#define SJSON__GT_RESERVED_KEYWORD_OFFSET ((long int) SJSON__T_RESERVED_KEYWORD_CONSTANT_FALSE)
#define SJSON__GT_SYMBOL_TOKEN_OFFSET ((long int) SJSON__T_SYMBOL_SIMPLE_COMMA)
#define SJSON__GT_LAST_RESERVED_TOKEN SJSON__T_RESERVED_KEYWORD_CONSTANT_TRUE
#define SJSON__GT_ESCAPE_CHARACTER ((char) 27)
#define SJSON__GT_CANCEL_CHARACTER ((char) 24)

#define SJSON__GT_RESERVED_KEYWORDS { \
    "false", "null", "true" \
}

#define SJSON__GT_VALID_SYMBOLS { \
    ',', ':', '[', ']', '{', '}' \
}


#ifndef SJSON__GT_TOKEN_BUFFER_INC_SIZE
#define SJSON__GT_TOKEN_BUFFER_INC_SIZE 64
#endif // TOKEN_BUFFER__INC_SIZE

#define SJSON__GT_ESCAPE_SEQ_MAX_LEN 5
#define SJSON__GT_REPRCHAR(CH) (CH == '\\' ? "\\\\" \
    : CH == '\'' ? "\\\'" \
    : CH == '\"' ? "\\\"" \
    : CH == '\a' ? "\\a"  \
    : CH == '\b' ? "\\b"  \
    : CH == '\f' ? "\\f"  \
    : CH == '\n' ? "\\n"  \
    : CH == '\r' ? "\\r"  \
    : CH == '\v' ? "\\v"  \
    : CH == '\t' ? "\\t" : (const char*)((char[]){ CH, '\0' }))

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DLLEXPORT
int sjson__gt_tokenize(const char* payload, struct sjson__token_t* token, char* error_message);

DLLEXPORT
int sjson__gt_free_tokens(struct sjson__token_t* token);

DLLEXPORT
char* sjson__gt_append_character(char* buffer, char ch);

DLLEXPORT
const char* sjson__gt_unicode_seq_validator(char ch, int remaining_char_count, char* esc_seq_str);

DLLEXPORT
int sjson__gt_get_esc_seq_char_count(char esc_seq_char);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_GENTOK_H__
