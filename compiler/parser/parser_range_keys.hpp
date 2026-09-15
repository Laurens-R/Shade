//
// Created by Laurens Ruijtenberg on 12/09/2026.
//

#ifndef SHADE_PARSER_RANGE_KEYS_HPP
#define SHADE_PARSER_RANGE_KEYS_HPP

namespace shade {
    /**
     * A centralized definition of keys used to identify parsed ranges.
     */
    struct range_keys {
        static inline const char * attribute_type_key = "attribute";
        static inline const char * attribute_indicator = "attribute_token";
        static inline const char * attribute_name = "attribute_name";
        static inline const char * attribute_arguments = "attribute_arguments";

        static inline const char * module_type_key = "module";
        static inline const char * module_keyword = "module_keyword";
        static inline const char * module_name = "module_name";
        static inline const char * module_body = "module_body";

        static inline const char * struct_type_key = "struct";
        static inline const char * struct_keyword = "struct_keyword";
        static inline const char * struct_name = "struct_name";
        static inline const char * struct_body = "struct_body";

        static inline const char * function_type_key = "function";
        static inline const char * function_keyword = "function_keyword";
        static inline const char * function_generic_arguments = "function_generic_arguments";
        static inline const char * function_return_type_indicator = "function_return_type_indicator";
        static inline const char * function_return_type = "function_return_type";
        static inline const char * function_name = "function_name";
        static inline const char * function_arguments = "function_arguments";
        static inline const char * function_body = "function_body";

        static inline const char * generic_type_key = "generic";
        static inline const char * generic_range = "generic_range";

        static inline const char * if_type_key = "if";
        static inline const char * if_keyword = "if_keyword";
        static inline const char * if_condition = "if_condition";
        static inline const char * if_body = "if_body";

        static inline const char * if_else_type_key = "if_else";
        static inline const char * if_else_keyword = "if_else_keyword";
        static inline const char * if_else_condition = "if_else_condition";
        static inline const char * if_else_body = "if_else_body";

        static inline const char * else_type_key = "else";
        static inline const char * else_keyword = "else_keyword";
        static inline const char * else_body = "else_body";

        static inline const char * for_type_key = "for";
        static inline const char * for_loop_keyword = "for_loop_keyword";
        static inline const char * for_loop_instructions = "for_loop_instructions";
        static inline const char * for_loop_body = "for_loop_body";

        static inline const char * while_type_key = "while";
        static inline const char * while_loop_keyword = "while_loop_keyword";
        static inline const char * while_loop_condition = "while_loop_condition";
        static inline const char * while_loop_body = "while_loop_body";

        static inline const char * raw_type_key = "raw";
        static inline const char * raw_scope = "raw_scope";

        static inline const char * statement_type_key = "statement";
        static inline const char * statement_expression = "statement_expression";

        static inline const char * type_expression_owning = "type_exp_owned";
        static inline const char * type_expression_constant = "type_exp_constant";
        static inline const char * type_expression_reference = "type_exp_reference";
        static inline const char * type_expression_pointer = "type_exp_pointer";
        static inline const char * type_expression_typename = "type_exp_typename";
        static inline const char * type_expression_fixed_array_range = "type_exp_fixed_array_range";
        static inline const char * type_expression_dynamic_array_range = "type_exp_dynamic_array_range";
        static inline const char * type_expression_generic_range = "type_exp_generic_range";

    };
}

#endif //SHADE_PARSER_RANGE_KEYS_HPP
