#ifndef __SJSON_GRAMMER_H__
#define __SJSON_GRAMMER_H__

#include "../common.h"
#include "sjson/@token.h"

#include <stddef.h>

typedef bool (*sjson__token_type_checker_t)(enum sjson__tokentype_t, struct sjson__context_t);

enum sjson__checkpoint_id_t {
    SJSON__CHECKPOINT_NONE = 0,
    SJSON__CHECKPOINT_OBJECT_START,
    SJSON__CHECKPOINT_OBJECT_END,
    SJSON__CHECKPOINT_ARRAY_START,
    SJSON__CHECKPOINT_ARRAY_END,
    SJSON__CHECKPOINT_SET_KEYNAME,
    SJSON__CHECKPOINT_KEY_VALUE_PAIR_END,
    SJSON__CHECKPOINT_VALUE
}; // enum sjson__checkpoint_id_t

typedef struct _sjson__grammer_rule_t {
    sjson__token_type_checker_t check_token_type_by;
    struct _sjson__grammer_rule_t* next_rules;
    const enum sjson__checkpoint_id_t checkpoint_id;
    const enum sjson__checkpoint_id_t goto_checkpoint_id;
} sjson__grammer_rule_t;

extern sjson__grammer_rule_t sjson_rule;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_GRAMMER_H__
