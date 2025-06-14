#include "sjson/main.h"
#include "sjson/@gentok.h"
#include <stdio.h>

DLLEXPORT
int sjson__parse(const char* payload, char* error_message) {
    if(!payload) return -1;
    struct sjson__token_t tokens = { 
        .token_type = SJSON__T_TOKEN_UNKNOWN,
        .token_buffer = NULL,
        .pos = 0UL,
        .next_token = NULL
    };
    size_t tc = sjson__gt_tokenize(payload, &tokens, error_message);
    if(error_message[0] == '\0') {
        printf("Number of tokens: %zu\n", tc);
        struct sjson__token_t* t = &tokens;
        for(long i = 0; i < tc; i++){
            printf("token {%s} found at %zu with type id %d\n", t->token_buffer, t->pos, (int) t->token_type);
            t = t->next_token;
        }
    }
    if(sjson__gt_free_tokens(&tokens) == tc) {
        puts("\nTokens freed");
    } else {
        puts("\nTokens could not be freed");
    }
    
    return 0;
}
