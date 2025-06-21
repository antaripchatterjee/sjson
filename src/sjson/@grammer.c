#include "sjson/@grammer.h"

sjson_rules = {
    .check_token_type_by = &sjson__is_object_start_token,
    .checkpoint_id = SJSON__CHECKPOINT_OBJECT_START,
    .goto_checkpoint_id = SJSON__CHECKPOINT_NONE,
    .next_rules = (sjson__grammer_rule_t[]) {
        {
            .check_token_type_by = &sjson__is_string_token,
            .checkpoint_id = SJSON__CHECKPOINT_SET_KEYNAME,
            .goto_checkpoint_id = SJSON__CHECKPOINT_NONE,
            .next_rules = (sjson__grammer_rule_t[]) {
                {
                    .check_token_type_by = &sjson__is_colon_token,
                    .checkpoint_id = SJSON__CHECKPOINT_NONE,
                    .goto_checkpoint_id = SJSON__CHECKPOINT_NONE,
                    .next_rules = (sjson__grammer_rule_t[]) {
                        {
                            .check_token_type_by = &sjson__is_value_token,
                            .checkpoint_id = SJSON__CHECKPOINT_NONE,
                            .goto_checkpoint_id = SJSON__CHECKPOINT_NONE,
                            .next_rules = (sjson__grammer_rule_t[]) {
                                {
                                    .check_token_type_by = &sjson__is_comma_token,
                                    .checkpoint_id = SJSON__CHECKPOINT_NONE,
                                    .goto_checkpoint_id = SJSON__CHECKPOINT_SET_KEYNAME,
                                    .next_rules = NULL
                                },
                                {0} // End of rules
                            }
                        },
                        {0} // End of rules
                    }
                },
                {0} // End of rules
            }
        },
        {
            .check_token_type_by = &sjson__is_object_end_token
        } // End of rules
    }
};