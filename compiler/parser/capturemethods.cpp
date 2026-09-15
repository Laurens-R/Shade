//
// Created by Laurens Ruijtenberg on 15/09/2026.
//

#include "capturemethods.hpp"

namespace shade {
    capture_methods::capture_methods() {
        init();
    }

    void capture_methods::init() {
        static bool are_ranges_initialized = false;

        if (are_ranges_initialized) {
            return;
        }

        attribute_range.match_and_capture_token(spelling::attributes::attr_indicator, range_keys::attribute_indicator)
                       .capture_tokens_by_count(1, range_keys::attribute_name)
                       .if_next_token_present(spelling::attributes::attr_begin_arguments)
                       .capture_range(spelling::attributes::attr_begin_arguments, spelling::attributes::attr_end_arguments, range_keys::attribute_arguments)
                       .set_type(range_keys::attribute_type_key);

        module_range.match_and_capture_token(spelling::modules::module_name, range_keys::module_keyword)
                    .capture_tokens_by_count(1, range_keys::module_name)
                    .capture_range(spelling::modules::begin_body, spelling::modules::end_body, range_keys::module_body)
                    .set_type(range_keys::module_type_key);

        struct_range.match_and_capture_token(spelling::structs::struct_indicator, range_keys::struct_keyword)
                    .optional_capture_range(spelling::generics::generic_begin, spelling::generics::generic_end, range_keys::generic_range)
                    .capture_tokens_by_count(1, range_keys::struct_name)
                    .capture_range(spelling::structs::begin_body, spelling::structs::end_body, range_keys::struct_body)
                    .set_type(range_keys::struct_type_key);

        function_range.match_and_capture_token(spelling::functions::func_indicator, range_keys::function_keyword)
                      .capture_tokens_by_count(1, range_keys::function_name)
                      .optional_capture_range(spelling::generics::generic_begin, spelling::generics::generic_end, range_keys::function_generic_arguments)
                      .capture_range(spelling::functions::begin_arguments, spelling::functions::end_arguments, range_keys::function_arguments)
                      .optional_match_and_capture_token(spelling::functions::func_return_type, range_keys::function_return_type_indicator)
                      .optional_take_until_if_previous_key(range_keys::function_return_type_indicator, spelling::functions::begin_body, range_keys::function_return_type)
                      .capture_range(spelling::functions::begin_body, spelling::functions::end_body, range_keys::function_body)
                      .set_type(range_keys::function_type_key);

        if_range.match_and_capture_token(spelling::conditional::if_start, range_keys::if_keyword)
                .capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_keys::if_condition)
                .capture_range(spelling::conditional::begin_body, spelling::conditional::end_body, range_keys::if_body)
                .set_type(range_keys::if_type_key);

        if_else_range.match_and_capture_token(spelling::conditional::if_else, range_keys::else_keyword)
                     .match_and_capture_token(spelling::conditional::if_start, range_keys::if_keyword)
                     .capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_keys::if_else_condition)
                     .capture_range(spelling::conditional::begin_body, spelling::conditional::end_body, range_keys::if_else_body)
                     .set_type(range_keys::if_else_type_key);

        else_range.match_and_capture_token(spelling::conditional::if_else, range_keys::else_keyword)
                  .capture_range(spelling::conditional::begin_body, spelling::conditional::end_body, range_keys::else_body)
                  .set_type(range_keys::else_type_key);

        for_loop_range.match_and_capture_token(spelling::loops::loop_for, range_keys::for_loop_keyword)
                      .capture_range(spelling::loops::begin_conditions, spelling::loops::end_conditions, range_keys::for_loop_instructions)
                      .capture_range(spelling::loops::begin_body, spelling::loops::end_body, range_keys::for_loop_body)
                      .set_type(range_keys::for_type_key);

        while_loop_range.match_and_capture_token(spelling::loops::loop_while, range_keys::while_loop_keyword)
                        .capture_range(spelling::loops::begin_conditions, spelling::loops::end_conditions, range_keys::while_loop_condition)
                        .capture_range(spelling::loops::begin_body, spelling::loops::end_body, range_keys::while_loop_body)
                        .set_type(range_keys::while_type_key);

        raw_scope_range.capture_range(spelling::scopes::begin_body, spelling::scopes::end_body, range_keys::raw_scope)
                       .set_type(range_keys::raw_type_key);

        statement_range.take_token_until(spelling::actions::end_of_statement, range_keys::statement_expression)
                       .set_type(range_keys::statement_type_key);

        are_ranges_initialized = true;
    }
} // shade