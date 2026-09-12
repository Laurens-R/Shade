//
// Created by laure on 10/09/2026.
//

#include "parser.h"

#include "range_capture.h"
#include "../lang/spelling.hpp"

namespace shade {

    struct range_keys {
        static inline const char * attribute_indicator = "attribute_indicator";
        static inline const char * attribute_name = "attribute_name";
        static inline const char * attribute_arguments = "attribute_arguments";

        static inline const char * module_indicator = "module_indicator";
        static inline const char * module_name = "module_name";
        static inline const char * module_body = "module_range";

        static inline const char * struct_indicator = "struct_indicator";
        static inline const char * struct_name = "struct_name";
        static inline const char * struct_body = "struct_range";

        static inline const char * function_indicator = "function_indicator";
        static inline const char * function_name = "function_name";
        static inline const char * function_arguments = "function_arguments";
        static inline const char * function_body = "function_body";

        static inline const char * if_indicator = "if_indicator";
        static inline const char * if_condition = "if_condition";
        static inline const char * if_body = "if_body";

        static inline const char * if_else_indicator = "if_else_indicator";
        static inline const char * if_else_condition = "if_else_condition";
        static inline const char * if_else_body = "if_else_body";

        static inline const char * else_indicator = "else_indicator";
        static inline const char * else_body = "else_body";

        static inline const char * for_loop_indicator = "for_loop_indicator";
        static inline const char * for_loop_instructions = "for_loop_instructions";
        static inline const char * for_loop_body = "for_loop_body";

        static inline const char * while_loop_indicator = "while_loop_indicator";
        static inline const char * while_loop_condition = "while_loop_condition";
        static inline const char * while_loop_body = "while_loop_body";

        static inline const char * raw_scope = "raw_scope_begin";
    };

    struct capture_methods {
        range_capture attribute_range;
        range_capture module_range;
        range_capture struct_range;
        range_capture function_range;
        range_capture if_range;
        range_capture if_else_range;
        range_capture else_range;
        range_capture for_loop_range;
        range_capture while_loop_range;
        range_capture raw_scope_range;
        range_capture statement_range;


        capture_methods()
        {
            init();
        }

        void init() {
            static bool are_ranges_initialized = false;

            if (are_ranges_initialized) {
                return;
            }

            attribute_range.match_and_capture_token(spelling::attributes::attr_indicator, range_keys::attribute_indicator)
            .capture_tokens_by_count(1, range_keys::attribute_name)
            .if_next_token_present(spelling::attributes::attr_begin_arguments)
            .capture_range(spelling::attributes::attr_begin_arguments, spelling::attributes::attr_end_arguments, range_keys::attribute_arguments);

            module_range.match_and_capture_token(spelling::modules::module_name, range_keys::module_indicator)
            .capture_tokens_by_count(1, range_keys::module_name)
            .capture_range(spelling::modules::begin_scope, spelling::modules::end_scope, range_keys::module_body);

            struct_range.match_and_capture_token(spelling::structs::struct_indicator, range_keys::struct_indicator)
            .capture_tokens_by_count(1, range_keys::struct_name)
            .capture_range(spelling::structs::struct_begin_scope, spelling::structs::struct_end_scope, range_keys::struct_body);

            function_range.match_and_capture_token(spelling::functions::func_indicator, range_keys::function_indicator)
            .capture_tokens_by_count(1, range_keys::function_name)
            .capture_range(spelling::functions::func_start_arguments, spelling::functions::func_end_arguments, range_keys::function_arguments)
            .capture_range(spelling::functions::func_begin_scope, spelling::functions::func_end_scope, range_keys::function_body);

            if_range.match_and_capture_token(spelling::conditional::if_start, range_keys::if_indicator)
            .capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_keys::if_condition)
            .capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_keys::if_body);

            if_else_range.match_and_capture_token(spelling::conditional::if_start, range_keys::if_else_indicator)
            .match_and_capture_token(spelling::conditional::if_else, range_keys::if_else_indicator)
            .capture_range(spelling::conditional::begin_condition_group, spelling::conditional::end_condition_group, range_keys::if_else_condition)
            .capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_keys::if_else_body);

            else_range.match_and_capture_token(spelling::conditional::if_else, range_keys::else_indicator)
            .capture_range(spelling::conditional::begin_scope, spelling::conditional::end_scope, range_keys::if_else_body);

            for_loop_range.match_and_capture_token(spelling::loops::loop_for, range_keys::for_loop_indicator)
            .capture_range(spelling::loops::start_conditions, spelling::loops::end_conditions, range_keys::for_loop_instructions)
            .capture_range(spelling::loops::begin_scope, spelling::loops::end_scope, range_keys::for_loop_body);

            while_loop_range.match_and_capture_token(spelling::loops::loop_while, range_keys::while_loop_indicator)
            .capture_range(spelling::loops::start_conditions, spelling::loops::end_conditions, range_keys::while_loop_condition)
            .capture_range(spelling::loops::begin_scope, spelling::loops::end_scope, range_keys::while_loop_body);

            raw_scope_range.capture_range(spelling::scopes::begin_scope, spelling::scopes::end_scope, range_keys::raw_scope);

            statement_range.skip_token_until(spelling::actions::end_of_statement);

            are_ranges_initialized = true;
        }
    };

    static const capture_methods & get_capture_methods() {
        static capture_methods methods;
        return methods;
    }

    parser::parser(compile_context& context)
        : _context(context)
    {
    }

    static size_t process_body_map(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> & results) {
        auto & capture_methods = get_capture_methods();
        auto current_token_index = from_token_index;

        while (current_token_index <= to_token_index) {
            /*
             * If-then-else statements
             */
            auto if_result = capture_methods.if_range.try_capture(tokens, current_token_index, to_token_index);
            if (if_result.matched) {
                auto if_body_result = if_result.get_captured_range(range_keys::if_body);
                if (if_body_result) {
                    auto & if_body = if_body_result.value();
                    process_body_map(tokens, if_body.from_token_index + 1, if_body.to_token_index - 1, if_result.child_results);
                }
                results.push_back(if_result);
                current_token_index = if_result.range_from_index + 1;

                /*
                 * else-if and else are nested into if because they can only be used after an if.
                 */

                auto if_else_result = capture_methods.if_else_range.try_capture(tokens, current_token_index, to_token_index);
                if (if_else_result.matched) {
                    auto if_else_body_result = if_else_result.get_captured_range(range_keys::if_else_body);
                    if (if_else_body_result) {
                        auto & if_else_body = if_else_body_result.value();
                        process_body_map(tokens, if_else_body.from_token_index + 1, if_else_body.to_token_index - 1, if_else_result.child_results);
                    }

                    results.push_back(if_else_result);
                    current_token_index = if_else_result.range_from_index + 1;
                }

                auto else_result = capture_methods.else_range.try_capture(tokens, current_token_index, to_token_index);
                if (else_result.matched) {
                    auto else_body_result = else_result.get_captured_range(range_keys::else_body);
                    if (else_body_result) {
                        auto & else_body = else_body_result.value();
                        process_body_map(tokens, else_body.from_token_index + 1, else_body.to_token_index - 1, else_result.child_results);
                    }

                    results.push_back(else_result);
                    current_token_index = else_result.range_from_index + 1;
                }

                continue;
            }

            /*
             * For loops
             */
            auto for_result = capture_methods.for_loop_range.try_capture(tokens, current_token_index, to_token_index);
            if (for_result.matched) {
                auto for_body_result = for_result.get_captured_range(range_keys::for_loop_body);
                if (for_body_result) {
                    auto & for_body = for_body_result.value();
                    process_body_map(tokens, for_body.from_token_index + 1, for_body.to_token_index - 1, for_result.child_results);
                }

                results.push_back(for_result);
                current_token_index = for_result.range_from_index + 1;

                continue;
            }

            /*
             * While loops
             */
            auto while_result = capture_methods.while_loop_range.try_capture(tokens, current_token_index, to_token_index);
            if (while_result.matched) {
                auto while_body_result = while_result.get_captured_range(range_keys::while_loop_body);
                if (while_body_result) {
                    auto & while_body = while_body_result.value();
                    process_body_map(tokens, while_body.from_token_index + 1, while_body.to_token_index - 1, while_result.child_results);
                }

                results.push_back(while_result);
                current_token_index = while_result.range_from_index + 1;
                continue;
            }

            /*
             * Regular statements
             */
            auto statement_result = capture_methods.statement_range.try_capture(tokens, current_token_index, to_token_index);
            if (statement_result.matched) {
                results.push_back(statement_result);
                current_token_index = statement_result.range_from_index + 1;
                continue;
            }
        }

        return current_token_index;
    }

    static size_t process_struct_map(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> & results) {
        auto & capture_methods = get_capture_methods();
        auto current_token_index = from_token_index;

        while (current_token_index <= to_token_index) {
            /*
             * Functions
             */
            auto function_result = capture_methods.function_range.try_capture(tokens, current_token_index, to_token_index);
            if (function_result.matched) {
                auto function_body_result = function_result.get_captured_range(range_keys::function_body);
                if (function_body_result) {
                    auto & module_body = function_body_result.value();
                    process_body_map(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, function_result.child_results);
                }
                results.push_back(function_result);
                current_token_index = function_result.range_from_index + 1;
                continue;
            }

            current_token_index = process_body_map(tokens, current_token_index, to_token_index, results);

            current_token_index++;
        }

        return current_token_index;
    }

    static void generate_parse_map(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> & results) {
        auto & capture_methods = get_capture_methods();

        auto current_token_index = from_token_index;

        while (current_token_index <= to_token_index) {

            /*
             * Modules
             */
            auto module_result = capture_methods.module_range.try_capture(tokens, current_token_index, to_token_index);
            if (module_result.matched) {
                auto module_body_result = module_result.get_captured_range(range_keys::module_body);
                if (module_body_result) {
                    auto & module_body = module_body_result.value();

                    //we recurse into this method for scopes because the global scope is a module.
                    generate_parse_map(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, module_result.child_results);
                }
                results.push_back(module_result);
                current_token_index = module_result.range_from_index + 1;
                continue;
            }

            /*
             * Structs
             */
            auto struct_result = capture_methods.struct_range.try_capture(tokens, current_token_index, to_token_index);
            if (struct_result.matched) {
                auto struct_body_result = struct_result.get_captured_range(range_keys::struct_body);
                if (struct_body_result) {
                    auto & struct_body = struct_body_result.value();
                    process_struct_map(tokens, struct_body.from_token_index + 1, struct_body.to_token_index - 1, struct_result.child_results);
                }
                results.push_back(struct_result);
                current_token_index = struct_result.range_from_index + 1;
                continue;
            }

            /*
             * Functions
             */
            auto function_result = capture_methods.function_range.try_capture(tokens, current_token_index, to_token_index);
            if (function_result.matched) {
                auto function_body_result = function_result.get_captured_range(range_keys::function_body);
                if (function_body_result) {
                    auto & module_body = function_body_result.value();
                    process_body_map(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, function_result.child_results);
                }
                results.push_back(function_result);
                current_token_index = function_result.range_from_index + 1;
                continue;
            }

            current_token_index = process_body_map(tokens, current_token_index, to_token_index, results);

            current_token_index++;
        }

    }

    void parser::parse(const std::vector<language_token>& tokens, const size_t from_token_index, const size_t to_token_index) {
        std::vector<capture_results> parse_map;
        generate_parse_map(tokens, from_token_index, to_token_index, parse_map);
    }
} // shade