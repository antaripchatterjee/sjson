#include "sjson/@token.h"


DLLEXPORT
bool sjson__is_boolean_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_RESERVED_KEYWORD_CONSTANT_TRUE ||
            token_type == SJSON__T_RESERVED_KEYWORD_CONSTANT_FALSE);
}

DLLEXPORT
bool sjson__is_null_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_RESERVED_KEYWORD_CONSTANT_NULL);
}

DLLEXPORT
bool sjson__is_string_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_OPERAND_STRING_VALUE);
}

DLLEXPORT
bool sjson__is_number_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_OPERAND_DECIMAL_INTEGER ||
            token_type == SJSON__T_OPERAND_DECIMAL_DOUBLE);
}

DLLEXPORT
bool sjson__is_value_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return sjson__is_boolean_token(token_type, context) ||
           sjson__is_null_token(token_type, context) ||
           sjson__is_string_token(token_type, context) ||
           sjson__is_number_token(token_type, context);
}

DLLEXPORT
bool sjson__is_array_start_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_SQRBRO);
}

DLLEXPORT
bool sjson__is_array_end_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_SQRBRC);
}

DLLEXPORT
bool sjson__is_object_start_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_CURLBRO);
}

DLLEXPORT
bool sjson__is_object_end_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_CURLBRC);
}

DLLEXPORT
bool sjson__is_comma_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_COMMA);
}

DLLEXPORT
bool sjson__is_colon_token(enum sjson__tokentype_t token_type, struct sjson__context_t* context){
    return (token_type == SJSON__T_SYMBOL_SIMPLE_COLON);
}