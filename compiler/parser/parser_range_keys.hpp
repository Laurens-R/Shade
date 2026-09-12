//
// Created by Laurens Ruijtenberg on 12/09/2026.
//

#ifndef SHADE_PARSER_RANGE_KEYS_HPP
#define SHADE_PARSER_RANGE_KEYS_HPP

namespace shade {
    struct range_keys {
        static inline const char * attribute_indicator = "attribute_token";
        static inline const char * attribute_name = "attribute_name";
        static inline const char * attribute_arguments = "attribute_arguments";

        static inline const char * module_keyword = "module_keyword";
        static inline const char * module_name = "module_name";
        static inline const char * module_body = "module_body";

        static inline const char * struct_keyword = "struct_keyword";
        static inline const char * struct_name = "struct_name";
        static inline const char * struct_body = "struct_body";

        static inline const char * function_indicator = "function_keyword";
        static inline const char * function_name = "function_name";
        static inline const char * function_arguments = "function_arguments";
        static inline const char * function_body = "function_body";

        static inline const char * if_indicator = "if_keyword";
        static inline const char * if_condition = "if_condition";
        static inline const char * if_body = "if_body";

        static inline const char * if_else_indicator = "if_else_keyword";
        static inline const char * if_else_condition = "if_else_condition";
        static inline const char * if_else_body = "if_else_body";

        static inline const char * else_indicator = "else_keyword";
        static inline const char * else_body = "else_body";

        static inline const char * for_loop_indicator = "for_loop_keyword";
        static inline const char * for_loop_instructions = "for_loop_instructions";
        static inline const char * for_loop_body = "for_loop_body";

        static inline const char * while_loop_indicator = "while_loop_keyword";
        static inline const char * while_loop_condition = "while_loop_condition";
        static inline const char * while_loop_body = "while_loop_body";

        static inline const char * raw_scope = "raw_scope";
    };
}

#endif //SHADE_PARSER_RANGE_KEYS_HPP
