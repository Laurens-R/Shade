//
// Created by laure on 10/09/2026.
//

#include "parser.h"

#include "range_capture.h"
#include "../lang/spelling.hpp"

namespace shade {

    struct capture_methods {
        static range_capture module_range;
        static range_capture struct_range;
        static range_capture function_range;
        static range_capture if_range;
        static range_capture if_else_range;
        static range_capture else_range;
        static range_capture for_loop_range;
        static range_capture while_loop_range;

        static void init() {
            static bool are_ranges_initialized = false;

            if (are_ranges_initialized) {
                return;
            }

            module_range.if_start_with(spelling::modules::module_name)
            .then_capture_range(spelling::modules::begin_scope, spelling::modules::end_scope, range_type::scope);

            struct_range.if_start_with(spelling::structs::struct_indicator)
            .then_capture_range(spelling::modules::begin_scope, spelling::modules::end_scope, range_type::scope);

            function_range.if_start_with(spelling::functions::func_indicator)
            .then_capture_range(spelling::functions::func_start_arguments, spelling::functions::func_end_arguments, range_type::group)
            .then_capture_range(spelling::functions::func_begin_scope, spelling::functions::func_end_scope, range_type::scope);

            if_range.if_start_with(spelling::conditional::if_start)
            .then_capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_type::group)
            .then_capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_type::scope);

            if_else_range.if_start_with(spelling::conditional::if_else)
            .then_capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_type::group)
            .then_capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_type::scope);

            else_range.if_start_with(spelling::conditional::if_else)
            .then_capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_type::scope);

            for_loop_range.if_start_with(spelling::loops::loop_for)
            .then_capture_range(spelling::loops::start_conditions, spelling::loops::end_conditions, range_type::group)
            .then_capture_range(spelling::loops::begin_scope, spelling::loops::end_scope, range_type::scope);

            while_loop_range.if_start_with(spelling::loops::loop_while)
            .then_capture_range(spelling::loops::start_conditions, spelling::loops::end_conditions, range_type::group)
            .then_capture_range(spelling::loops::begin_scope, spelling::loops::end_scope, range_type::scope);

            are_ranges_initialized = true;
        }
    };

    static const capture_methods & get_capture_methods() {
        static capture_methods methods;
        shade::capture_methods::init();
        return methods;
    }
    static capture_methods capture_ranges;

    void parser::try_scope_captures(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index) {
        const auto & capture = get_capture_methods();

        /* Module block */
        auto found_module = capture.module_range.try_capture(tokens, from_token_index, to_token_index);
        if (found_module.has_value()) {
            auto & module_range = found_module.value();

            //todo: parse stuff

            parse(tokens, module_range.from_token_index, module_range.to_token_index);
        }

        /* Struct block */
        auto found_struct = capture.struct_range.try_capture(tokens, from_token_index, to_token_index);
        if (found_struct.has_value()) {
            auto & struct_range = found_struct.value();

            //todo: parse stuff

            parse(tokens, struct_range.from_token_index, struct_range.to_token_index);
        }

        /* Function block */
        auto found_function = capture.function_range.try_capture(tokens, from_token_index, to_token_index);
        if (found_function.has_value()) {
            auto & function_range = found_function.value();

            //todo: parse stuff

            parse(tokens, function_range.from_token_index, function_range.to_token_index);
        }

        /* conditional block */
        auto found_conditional = capture.if_range.try_capture(tokens, from_token_index, to_token_index);
        if (found_conditional.has_value()) {
            auto & if_range = found_conditional.value();

            //todo: parse stuff

            parse(tokens, if_range.from_token_index, if_range.to_token_index);

            // scan for else (some_condition) { ... } blocks
            auto found_else_conditional = capture.if_else_range.try_capture(tokens, from_token_index, to_token_index);

            while (found_else_conditional.has_value()) {
                auto & else_conditional_range = found_else_conditional.value();

                //todo: parse stuff
                parse(tokens, else_conditional_range.from_token_index, else_conditional_range.to_token_index);
                found_else_conditional = capture.if_else_range.try_capture(tokens, from_token_index, to_token_index);
            }

            // and finally check if an else { ... } block has been defined
            auto found_else = capture.else_range.try_capture(tokens, from_token_index, to_token_index);
            if (found_else.has_value()) {
                auto & else_range = found_else.value();

                //todo: parse stuff
                parse(tokens, else_range.from_token_index, else_range.to_token_index);
            }
        }
    }

    void parser::parse(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index) {
        try_scope_captures(tokens, from_token_index, to_token_index);
    }
} // shade