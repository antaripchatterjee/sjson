#include "sjson/@gentok.h"
#include "sjson/@error.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static struct sjson__token_t* sjson__gt_add_token(struct sjson__token_t* token, const char* token_buffer, size_t pos, enum sjson__tokentype_t token_type) {
    token->token_type = token_type;
    token->token_buffer = (char*) malloc(sizeof(char) * (strlen(token_buffer) + 1));
    strcpy(token->token_buffer, token_buffer);
    token->token_buffer[strlen(token_buffer)] = '\0';
    token->pos = pos;
    token->next_token = (struct sjson__token_t*) malloc(sizeof(struct sjson__token_t));
    token->next_token->token_type = SJSON__T_TOKEN_UNKNOWN;
    token->next_token->token_buffer = NULL;
    token->next_token->next_token = NULL;
    return token->next_token;
}

char* sjson__gt_append_character(char* buffer, char ch) {
    size_t len = buffer ? strlen(buffer) : 0;
    if(buffer == NULL || (len > 0 && len % SJSON__GT_TOKEN_BUFFER_INC_SIZE == 0)) {
        buffer = (char*) realloc(buffer, sizeof(char) * (len + SJSON__GT_TOKEN_BUFFER_INC_SIZE + 1));
        if(!buffer) {
            return NULL;
        }
        memset(buffer + len, '\0', sizeof(char) * (SJSON__GT_TOKEN_BUFFER_INC_SIZE + 1));
    }
    buffer[len] = ch;
    return buffer;
}

const char* sjson__gt_unicode_seq_validator(char ch, int remaining_char_count, char* esc_seq_str) {
    if(isxdigit(ch)) {
        int pos = 4 - remaining_char_count;
        if(pos < 0) {
            return "Invalid position for universal character name";
        }
        esc_seq_str[pos] = ch;
    } else {
        return "Incomplete universal character name";
    }
    return NULL;
}


int sjson__gt_get_esc_seq_char_count(char esc_seq_char) {
    if(esc_seq_char == '\\' || esc_seq_char == '"' 
        || esc_seq_char == '/' || esc_seq_char == 'b'
        || esc_seq_char == 'f' || esc_seq_char == 'n'
        || esc_seq_char == 'r' || esc_seq_char == 't') {
        return 0;
    } else if(esc_seq_char == 'u') {
        return 4;
    } else {
        return -1;
    }
}

size_t sjson__gt_tokenize(const char* payload, struct sjson__token_t* token, char* error_message) {
    if(!payload || !token || !error_message) {
        return 0UL;
    }
    const char* reserved_keywords[] = SJSON__GT_RESERVED_KEYWORDS;
    const char valid_symbols[] = SJSON__GT_VALID_SYMBOLS;
    size_t number_of_tokens = 0UL;
    // char error_message[SJSON__ERROR_MSG_SIZE+1] = { 0 };

    const long count_reserved_keywords = sizeof(reserved_keywords)/sizeof(char*);
    const long count_valid_symbols = sizeof(valid_symbols)/sizeof(char);

    char curr_char, prev_char = '\0';

    // bool success = true;

    bool is_number = false, is_number_beginning = false, 
        is_resv_const = false, is_string = false;
    bool exp_appended = false, decimal_appended = false, allow_char_as_num = false;
    bool push_current_token = false;
    
    enum sjson__tokentype_t current_token_type = SJSON__T_TOKEN_UNKNOWN; //, prev_token_type;
    struct sjson__token_t* current_token = token;
    char* token_buffer = NULL;

    char esc_seq_str[SJSON__GT_ESCAPE_SEQ_MAX_LEN] = { 0 };
    int escape_seq_char_count = -1;
    bool is_unicode_seq = false;

    for(size_t index = 0; (curr_char = payload[index]) != '\0' && error_message[0] == '\0'; index++) {
        if(isprint(curr_char) || isspace(curr_char)) {
            if(is_string) {
                if(iscntrl(curr_char)) {
                    sjson__build_error_message(error_message, index, "Found control character '%s' (ASCII %d) in string literal", SJSON__GT_REPRCHAR(curr_char), curr_char);
                }
                if(error_message[0] == '\0') {
                    token_buffer = sjson__gt_append_character(token_buffer, curr_char);
                    if(prev_char == SJSON__GT_ESCAPE_CHARACTER) {
                        if(escape_seq_char_count > 0) {
                            const char* fixed_error_msg = sjson__gt_unicode_seq_validator(curr_char, escape_seq_char_count, esc_seq_str);
                            if(fixed_error_msg) {
                                sjson__build_error_message(error_message, index, "Invalid string literal. %s %s.", fixed_error_msg, esc_seq_str);
                            }
                            escape_seq_char_count--;
                        } else {
                            escape_seq_char_count = sjson__gt_get_esc_seq_char_count(curr_char);
                            if(escape_seq_char_count < 0) {
                                sjson__build_error_message(error_message, index, 
                                    "Invalid string literal. Invalid escape sequence '\\%c'.", curr_char);
                            } else if(escape_seq_char_count == 4) {
                                is_unicode_seq = true;
                            }
                        }
                        if(is_unicode_seq && !escape_seq_char_count) {
                            memset(esc_seq_str, 0, SJSON__GT_ESCAPE_SEQ_MAX_LEN);
                            escape_seq_char_count = -1;
                            is_unicode_seq = false;
                            prev_char = '\0';
                        }
                    }
                    if(curr_char == '\\' && prev_char != SJSON__GT_ESCAPE_CHARACTER) {
                        curr_char = SJSON__GT_ESCAPE_CHARACTER;
                    } else if(curr_char == '"' && prev_char != SJSON__GT_ESCAPE_CHARACTER) {
                        current_token_type = SJSON__T_OPERAND_STRING_VALUE;
                        push_current_token = true;
                        index++;
                        is_string = false;
                    }
                    if(escape_seq_char_count > 0) {
                        curr_char = SJSON__GT_ESCAPE_CHARACTER;
                    }
                }
            } else {
                if(is_resv_const) {
                    if(isalnum(curr_char) || curr_char == '_') {
                        token_buffer = sjson__gt_append_character(token_buffer, curr_char);
                    } else {
                        // long token_index = get_token_index(token_buffer, count_reserved_keywords, reserved_keywords);
                        int token_index = -1;
                        for(int i = 0; i < count_reserved_keywords; i++) {
                            if(strcmp(reserved_keywords[i], token_buffer) == 0) {
                                token_index = i;
                                break;
                            }
                        }
                        if(token_index != -1) {
                            current_token_type = (enum sjson__tokentype_t) (token_index + SJSON__GT_RESERVED_KEYWORD_OFFSET);
                        } else {
                            sjson__build_error_message(error_message, index, "Invalid reserved keyword '%s'.", token_buffer);
                        }
                        is_resv_const = false;
                        push_current_token = true;
                    }
                } else if(is_number) {
                    if(is_number_beginning) {
                        is_number_beginning = false;
                        if(prev_char == '0') {
                            if(curr_char == '.' || tolower(curr_char) == 'e') {
                                decimal_appended = true;
                                if(curr_char == 'e') {
                                    exp_appended = true;
                                }
                            } else if(curr_char != ',' && curr_char != '}' && !isspace(curr_char)) {
                                allow_char_as_num = false;
                                sjson__build_error_message(error_message, index,
                                    "Unexpected occurence of '%s' (ASCII %d), "
                                    "not a valid decimal digit after leading zero.\n", SJSON__GT_REPRCHAR(curr_char), curr_char
                                );
                            } else {
                                allow_char_as_num = false;
                            }
                        } else {
                            if(isdigit(curr_char) || curr_char == '.' || tolower(curr_char) == 'e') {
                                if(curr_char == '.') {
                                    decimal_appended = true;
                                } else if(tolower(curr_char) == 'e') {
                                    decimal_appended = true;
                                    exp_appended = true;
                                }
                            } else if(curr_char != ',' && curr_char != '}' && !isspace(curr_char)) {
                                // Raise invalid syntax error
                                allow_char_as_num = false;
                                sjson__build_error_message(error_message, index,
                                    "Unexpected occurence of '%s' (ASCII %d), "
                                    "not a valid decimal digit.\n", SJSON__GT_REPRCHAR(curr_char), curr_char
                                );
                            } else {
                                allow_char_as_num = false;
                            }
                        }
                    } else {
                        if(isdigit(curr_char) || (curr_char == '.' && !decimal_appended)
                            || (tolower(curr_char) == 'e' && !exp_appended)
                            || ((curr_char == '+' || curr_char == '-') && tolower(prev_char) == 'e' && exp_appended)) {
                            if(curr_char == '.') {
                                decimal_appended = true;
                            } else if(tolower(curr_char) == 'e') {
                                decimal_appended = true;
                                exp_appended = true;
                            }
                        } else if(curr_char != ',' && curr_char != '}' && !isspace(curr_char)) {
                            // Raise the invalid syntax error
                            allow_char_as_num = false;
                            sjson__build_error_message(error_message, index,
                                "Unexpected occurence of '%s' (ASCII %d), "
                                "not a valid decimal digit.", SJSON__GT_REPRCHAR(curr_char), curr_char
                            );
                        } else {
                            allow_char_as_num = false;
                        }
                    }
                    if(allow_char_as_num) {
                        token_buffer = sjson__gt_append_character(token_buffer, tolower(curr_char));
                    } else {
                        current_token_type = exp_appended || decimal_appended 
                            ? SJSON__T_OPERAND_DECIMAL_DOUBLE : SJSON__T_OPERAND_DECIMAL_INTEGER;
                        push_current_token = true;
                        is_number = false;
                        exp_appended = decimal_appended = allow_char_as_num = false;
                    }
                } else {
                    if(isalpha(curr_char) || curr_char == '_') {
                        token_buffer = sjson__gt_append_character(token_buffer, tolower(curr_char));
                        is_resv_const = true;
                    } else if(isdigit(curr_char) || curr_char == '-') {
                        token_buffer = sjson__gt_append_character(token_buffer, curr_char);
                        is_number = true;
                        is_number_beginning = true;
                        allow_char_as_num = true;
                    } else if(curr_char == '"') {
                        token_buffer = sjson__gt_append_character(token_buffer, curr_char);
                        is_string = true;
                    } else if(!isspace(curr_char)) {
                        // long token_index = get_token_index(token_buffer, count_valid_symbols, valid_symbols);
                        int token_index = -1;
                        for(int i = 0; i < count_valid_symbols; i++) {
                            if(valid_symbols[i] == curr_char) {
                                token_index = i;
                                break;
                            }
                        }
                        if(token_index != -1) {
                            current_token_type = (enum sjson__tokentype_t) (token_index + SJSON__GT_SYMBOL_TOKEN_OFFSET);
                            token_buffer = sjson__gt_append_character(token_buffer, curr_char);
                            push_current_token = true;
                            index++;
                        } else {
                            // Raise invalid token error
                            sjson__build_error_message(error_message, index,
                                "Found \"%s\", not a valid symbol.", token_buffer
                            );
                        }
                    }
                }
            }
        } else {
            sjson__build_error_message(error_message, index, 
                "Found invalid or unsupported control character '%s' (ASCII %d)\n", SJSON__GT_REPRCHAR(curr_char), curr_char);
        }
        if(push_current_token) {
            current_token = sjson__gt_add_token(current_token, (const char*) token_buffer, --index, current_token_type);
            number_of_tokens++;
            memset(token_buffer, 0, sizeof(char) * (strlen(token_buffer) + 1));
            push_current_token = false;
            current_token_type = SJSON__T_TOKEN_UNKNOWN;
            prev_char = curr_char == '\r' ? '\r' : '\0';
        } else {
            prev_char = curr_char;
        }
    }
    if (error_message[0] == '\0' && is_string) {
        sjson__build_error_message(error_message, strlen(payload)-1, "Invalid string literal, missing string ending character \"");
    }
    if(token_buffer) free(token_buffer);
    return number_of_tokens;
}

size_t sjson__gt_free_tokens(struct sjson__token_t* token) {
    struct sjson__token_t* _next_token = token;
    size_t number_of_tokens = 0UL;
    while(_next_token) {
        struct sjson__token_t* _temp_token = _next_token;
        _next_token = _next_token->next_token;
        if(_temp_token->token_buffer) {
            free(_temp_token->token_buffer);
            number_of_tokens++;
        }
        if(_temp_token != token) free(_temp_token);
    }
    return number_of_tokens;
}