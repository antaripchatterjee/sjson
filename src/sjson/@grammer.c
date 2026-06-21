#include "sjson/@grammer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int* push_g_state(int* g_state_queue, size_t *len, size_t *cap, int g_state) {
    if(*len == * cap) {
        int* g_state_queue_temp = (int*) realloc(g_state_queue, sizeof(int) * ((*cap) + 512));
        if(!g_state_queue_temp) {
            *len = 0;
            *cap = 0;
            free(g_state_queue);
            g_state_queue = NULL;
        } else {
            g_state_queue = g_state_queue_temp;
            memset(g_state_queue + *cap, 0, 512 * sizeof(int));
            *cap += 512;
        }
    }
    if(*cap) {
        g_state_queue[*len] = g_state;
        *len = *len + 1;
    }
    return g_state_queue;
}


DLLEXPORT
int sjson__validate_grammer(struct sjson__token_t* tokens, struct sjson__context_t* context, char* error_message) {
    if(!tokens || !context) {
        return -1;
    }
    struct sjson__token_t* token = tokens;
    size_t g_state_queue_cap = 512;
    size_t g_state_queue_len = 1; // at g_state_queue[0] => SJSON__G_STATE_CTX_BEGIN
    int *g_state_queue = (int*) malloc(sizeof(int) * g_state_queue_cap);
    if(!g_state_queue) {
        return -2;
    }
    memset(g_state_queue, 0, sizeof(int) * g_state_queue_cap);

    char err[SJSON__ERROR_MSG_SIZE + 1] = { 0 };
    int g_state = SJSON__G_STATE_CTX_BEGIN;

    do {
        printf("token {%s} found at %zu with type id %d\n", token->token_buffer, token->pos, (int) token->token_type);
        switch(g_state) {
            case SJSON__G_STATE_CTX_BEGIN:
            if(sjson__is_object_start_token(token->token_type)) {
                g_state = SJSON__G_STATE_OBJ_BEGIN;
            } else if(sjson__is_array_start_token(token->token_type)) {
                g_state = SJSON__G_STATE_ARR_BEGIN;
            } else {
                // set error message
                g_state = SJSON__G_STATE_ERROR;
            }
            if(g_state != SJSON__G_STATE_ERROR && (g_state_queue = push_g_state(g_state_queue, &g_state_queue_len, &g_state_queue_cap, g_state)) == NULL) {
                // set error message
                g_state = SJSON__G_STATE_ERROR;
            }
            break;
            case SJSON__G_STATE_OBJ_BEGIN:
            if(sjson__is_string_token(token->token_type)) {
                g_state = SJSON__G_STATE_KEYVAL_BEGIN;
            } else if(sjson__is_object_end_token(token->token_type) && g_state_queue[g_state_queue_len-1] == SJSON__G_STATE_OBJ_BEGIN) {
                // do something
                g_state_queue_len--;
                if(g_state_queue[g_state_queue_len-1] == SJSON__G_STATE_KEYVAL_END) {
                    g_state = SJSON__G_STATE_KEYVAL_END;
                }
            } else {
                // set error message
                g_state = SJSON__G_STATE_ERROR;
            }
            if(g_state_queue[g_state_queue_len-1] == SJSON__G_STATE_KEYVAL_END) {
                g_state_queue_len--;
            }
            break;
            case SJSON__G_STATE_KEYVAL_BEGIN:
            if(sjson__is_colon_token(token->token_type)) {
                g_state = SJSON__G_STATE_KEYVAL_SET;
            } else {
                // set error message
            }
            break;
            case SJSON__G_STATE_KEYVAL_SET:
            if(sjson__is_literal_token(token->token_type)) {
                g_state = SJSON__G_STATE_KEYVAL_END;
            } else if (sjson__is_object_start_token(token->token_type)) {
                if((g_state_queue = push_g_state(g_state_queue, &g_state_queue_len, &g_state_queue_cap, SJSON__G_STATE_KEYVAL_END)) == NULL) {
                    // set the error
                    g_state = SJSON__G_STATE_ERROR;
                } else {
                    g_state = SJSON__G_STATE_OBJ_BEGIN;
                    if((g_state_queue = push_g_state(g_state_queue, &g_state_queue_len, &g_state_queue_cap, g_state)) == NULL) {
                        // set the error
                        g_state = SJSON__G_STATE_ERROR;
                    }
                }
            } else if (sjson__is_array_start_token(token->token_type)) {
                // do something
            } else {
                // set the error
            }
            break;
            case SJSON__G_STATE_KEYVAL_END:
            if(sjson__is_comma_token(token->token_type)) {
                if((g_state_queue = push_g_state(g_state_queue, &g_state_queue_len, &g_state_queue_cap, g_state)) == NULL) {
                    // set the error
                    g_state = SJSON__G_STATE_ERROR;
                } else {
                    g_state = SJSON__G_STATE_OBJ_BEGIN;
                }
            } else if (sjson__is_object_end_token(token->token_type) 
                && g_state_queue[g_state_queue_len-1] == SJSON__G_STATE_OBJ_BEGIN) {
                g_state_queue_len--;
                if(g_state_queue[g_state_queue_len-1] == SJSON__G_STATE_KEYVAL_END) {
                    g_state_queue_len--;
                }
                // do something
            } else {
                // set the error message
            }
            break;
            default:
            break;
        }
        token = token->next_token;
    } while (token && token->token_type != SJSON__T_TOKEN_UNKNOWN && g_state != SJSON__G_STATE_ERROR);
    if(g_state_queue) {
        free(g_state_queue);
    } 
    if(error_message && err[0] != '\0') {
        strncpy(error_message, err, strlen(err));
        return -3;
    }
    return 0;
}