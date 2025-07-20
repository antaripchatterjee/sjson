#include "sjson/main.h"
#include "sjson/@gentok.h"
#include "sjson/@grammer.h"
#include <stdio.h>

DLLEXPORT
int sjson__parse(const char* payload, char* error_message) {
    if(!payload || !error_message) return -1;
    struct sjson__token_t tokens = { 
        .token_type = SJSON__T_TOKEN_UNKNOWN,
        .token_buffer = NULL,
        .pos = 0UL,
        .next_token = NULL
    };
    int arg_err = sjson__gt_tokenize(payload, &tokens, error_message);
    if(!arg_err && error_message && error_message[0] == '\0') {
        arg_err = sjson__validate_grammer(&tokens, NULL, error_message);
        if(!arg_err && error_message && error_message[0] == '\0') {
            // Do something
        } else {
            puts(error_message);
        }
    } else {
        puts(error_message);
    }
    if(!sjson__gt_free_tokens(&tokens)) {
        puts("Tokens freed");
    } else {
        puts("Tokens could not be freed");
    }
    
    return 0;
}
