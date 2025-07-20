#ifndef __SJSON_GRAMMER_H__
#define __SJSON_GRAMMER_H__

#include "../common.h"
#include "sjson/@token.h"
#include "sjson/@context.h"

enum sjson__grammer_state {
    SJSON__G_STATE_ERROR = -1,
    SJSON__G_STATE_CTX_BEGIN,
    SJSON__G_STATE_OBJ_BEGIN,
    SJSON__G_STATE_ARR_BEGIN,
    SJSON__G_STATE_KEYVAL_BEGIN,
    SJSON__G_STATE_KEYVAL_SET,
    SJSON__G_STATE_KEYVAL_END,
    SJSON__G_STATE_KEY_BEGIN,
    SJSON__G_STATE_VAL_BEGIN,
    SJSON__G_STATE_VAL_END,
    SJSON__G_STATE_KEY_END,
    SJSON__G_STATE_ITEM_BEGIN,
    SJSON__G_STATE_ITEM_END
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DLLEXPORT
int sjson__validate_grammer(struct sjson__token_t* tokens, struct sjson__context_t* context, char* error_message);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SJSON_GRAMMER_H__
