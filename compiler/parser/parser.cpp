//
// Created by laure on 10/09/2026.
//

#include "parser.hpp"

#include "range_capture.hpp"
#include "../lang/spelling.hpp"
#include "parser_range_keys.hpp"
#include "../meta/module_definition.hpp"
#include "../meta/function_definition.hpp"

namespace shade {
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

        capture_methods() {
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

            statement_range.skip_token_until(spelling::actions::end_of_statement)
                           .set_type(range_keys::generic_type_key);

            are_ranges_initialized = true;
        }
    };

    static const capture_methods &get_capture_methods() {
        static capture_methods methods;
        return methods;
    }

    parser::parser(compile_context &context, const std::vector<language_token> &tokens)
        : _context(context), _tokens(tokens) {}

    static size_t process_statements(const std::vector<language_token> &tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> &results) {
        auto &capture_methods = get_capture_methods();

        auto current_token_index = from_token_index;

        while (current_token_index < to_token_index) {
            /*
             * If-then-else statements
             */
            auto if_result = capture_methods.if_range.try_capture(tokens, current_token_index, to_token_index);
            if (if_result.matched) {
                auto if_body_result = if_result.get_captured_range(range_keys::if_body);
                if (if_body_result) {
                    auto &if_body = if_body_result.value();
                    process_statements(tokens, if_body.from_token_index + 1, if_body.to_token_index - 1, if_result.child_results);
                }
                results.push_back(if_result);
                current_token_index = if_result.range_to_index;

                /*
                 * else-if and else are nested into if because they can only be used after an if.
                 */

                auto if_else_result = capture_methods.if_else_range.try_capture(tokens, current_token_index, to_token_index);
                if (if_else_result.matched) {
                    auto if_else_body_result = if_else_result.get_captured_range(range_keys::if_else_body);
                    if (if_else_body_result) {
                        auto &if_else_body = if_else_body_result.value();
                        process_statements(tokens, if_else_body.from_token_index + 1, if_else_body.to_token_index - 1, if_else_result.child_results);
                    }

                    results.push_back(if_else_result);
                    current_token_index = if_else_result.range_to_index;
                }

                auto else_result = capture_methods.else_range.try_capture(tokens, current_token_index, to_token_index);
                if (else_result.matched) {
                    auto else_body_result = else_result.get_captured_range(range_keys::else_body);
                    if (else_body_result) {
                        auto &else_body = else_body_result.value();
                        process_statements(tokens, else_body.from_token_index + 1, else_body.to_token_index - 1, else_result.child_results);
                    }

                    results.push_back(else_result);
                    current_token_index = else_result.range_to_index;
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
                    auto &for_body = for_body_result.value();
                    process_statements(tokens, for_body.from_token_index + 1, for_body.to_token_index - 1, for_result.child_results);
                }

                results.push_back(for_result);
                current_token_index = for_result.range_to_index + 1;

                continue;
            }

            /*
             * While loops
             */
            auto while_result = capture_methods.while_loop_range.try_capture(tokens, current_token_index, to_token_index);
            if (while_result.matched) {
                auto while_body_result = while_result.get_captured_range(range_keys::while_loop_body);
                if (while_body_result) {
                    auto &while_body = while_body_result.value();
                    process_statements(tokens, while_body.from_token_index + 1, while_body.to_token_index - 1, while_result.child_results);
                }

                results.push_back(while_result);
                current_token_index = while_result.range_to_index + 1;
                continue;
            }

            /*
             * Regular statements
             */
            auto statement_result = capture_methods.statement_range.try_capture(tokens, current_token_index, to_token_index);
            if (statement_result.matched) {
                results.push_back(statement_result);
                current_token_index = statement_result.range_to_index + 1;
                continue;
            }
        }

        return current_token_index;
    }

    static size_t process_struct_map(const std::vector<language_token> &tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> &results) {
        auto &capture_methods = get_capture_methods();

        auto current_token_index = from_token_index;

        while (current_token_index <= to_token_index) {
            /*
             * Functions
             */
            auto function_result = capture_methods.function_range.try_capture(tokens, current_token_index, to_token_index);
            if (function_result.matched) {
                auto function_body_result = function_result.get_captured_range(range_keys::function_body);
                if (function_body_result) {
                    auto &module_body = function_body_result.value();
                    process_statements(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, function_result.child_results);
                }
                results.push_back(function_result);
                current_token_index = function_result.range_to_index + 1;
                continue;
            }

            current_token_index = process_statements(tokens, current_token_index, to_token_index, results);

            current_token_index++;
        }

        return current_token_index;
    }

    static void generate_parse_map(const std::vector<language_token> &tokens, const size_t from_token_index, const size_t to_token_index, std::vector<capture_results> &results) {
        //this can happen in case a passed down subscope is empty (e.g. a function with no body)
        //you want to call generate_parse_map with +1 from_token_index and -1 to_token_index to
        //ensure that you don't include the begin and end tokens of the subscope. However
        //this causes that from_token_index > to_token_index.
        if (from_token_index > to_token_index) {
            return;
        }

        auto &capture_methods = get_capture_methods();
        auto current_token_index = from_token_index;

        while (current_token_index <= to_token_index) {
            /*
             * Modules
             */
            auto module_result = capture_methods.module_range.try_capture(tokens, current_token_index, to_token_index);
            if (module_result.matched) {
                auto module_body_result = module_result.get_captured_range(range_keys::module_body);
                if (module_body_result) {
                    auto &module_body = module_body_result.value();

                    //we recurse into this method for scopes because these can be nested
                    //note the the concequence of thsi is the from_token_index > to_token_index
                    //situation as outlined above.
                    generate_parse_map(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, module_result.child_results);
                }
                results.push_back(module_result);
                current_token_index = module_result.range_to_index;
                continue;
            }

            /*
             * Structs
             */
            auto struct_result = capture_methods.struct_range.try_capture(tokens, current_token_index, to_token_index);
            if (struct_result.matched) {
                auto struct_body_result = struct_result.get_captured_range(range_keys::struct_body);
                if (struct_body_result) {
                    auto &struct_body = struct_body_result.value();
                    process_struct_map(tokens, struct_body.from_token_index + 1, struct_body.to_token_index - 1, struct_result.child_results);
                }
                results.push_back(struct_result);
                current_token_index = struct_result.range_to_index;
                continue;
            }

            /*
             * Functions
             */
            auto function_result = capture_methods.function_range.try_capture(tokens, current_token_index, to_token_index);
            if (function_result.matched) {
                auto function_body_result = function_result.get_captured_range(range_keys::function_body);
                if (function_body_result) {
                    auto &module_body = function_body_result.value();
                    process_statements(tokens, module_body.from_token_index + 1, module_body.to_token_index - 1, function_result.child_results);
                }
                results.push_back(function_result);
                current_token_index = function_result.range_to_index;
                continue;
            }

            current_token_index = process_statements(tokens, current_token_index, to_token_index, results);

            current_token_index++;
        }
    }

    std::vector<capture_results> &parser::capture_main_areas() {
        _code_map.clear();
        generate_parse_map(_tokens, 0, _tokens.size() - 1, _code_map);
        return _code_map;
    }

    static void analyze_map(std::vector<capture_results> &captured_information, const std::vector<language_token> &tokens, compile_context &context, capture_results *parent = nullptr, module_definition *parent_module = nullptr, type_definition * parent_type = nullptr) {
        auto &program_structure = context.get_program_structure();

        for (auto &result: captured_information) {
            if (result.type_key == range_keys::struct_type_key) {
                auto struct_result = result.get_captured_range(range_keys::struct_name);
                if (struct_result) {
                    auto &struct_information = struct_result.value();

                    auto &original_token = tokens[struct_information.from_token_index];
                    auto name = original_token.text;

                    if (parent_module != nullptr) {
                        name = path_utils::remove_global_prefix(parent_module->full_path + spelling::modules::module_seperator + name);
                    }

                    if (program_structure.contains_type_definition(name)) {
                        context.log_error(original_token.line, original_token.column, "Struct with name already exists.");
                        return;
                    }

                    auto new_type = type_definition::create_struct(name, nullptr);
                    auto added_type = program_structure.add_type_definition(new_type);
                    added_type->related_token_capture = &result;

                    analyze_map(result.child_results, tokens, context, &result, parent_module, added_type);;
                } else {
                    auto &original_token = tokens[result.range_from_index];
                    context.log_error(original_token.line, original_token.column, "Error during parsing of struct.");
                }
            }

            if (result.type_key == range_keys::module_type_key) {
                auto module_result = result.get_captured_range((range_keys::module_name));

                if (module_result) {
                    auto &module_name = tokens[module_result.value().from_token_index].text;
                    module_definition *added_module = nullptr;

                    if (parent_module == program_structure.get_global_module()) {
                        added_module = program_structure.add_module_definition(module_name);
                    } else {
                        auto full_path = parent_module->full_path + spelling::modules::module_seperator + module_name;
                        added_module = program_structure.add_module_definition(full_path);
                    }

                    added_module->related_token_capture = &result;
                    analyze_map(result.child_results, tokens, context, &result, added_module);
                }
            }

            if (result.type_key == range_keys::function_type_key) {
                auto function_result = result.get_captured_range((range_keys::function_name));

                if (function_result) {
                    auto &function_name = tokens[function_result.value().from_token_index].text;

                    function_definition *created_function = nullptr;

                    if (parent_module == nullptr) continue;

                    //parent could be null in case we have a function definition in the global scope
                    if (parent == nullptr || parent->type_key == range_keys::module_type_key) {
                        //it's a function in a module or the global scope!
                        cstring full_function_name = cstring::empty();

                        //if the parent module has no path, then it's a global module.
                        if (parent_module->full_path == cstring::empty()) {
                            full_function_name = function_name;
                        } else {
                            full_function_name = parent_module->full_path + spelling::modules::module_seperator + function_name;
                        }

                        function_definition func_def = function_definition::create(full_function_name, false);
                        func_def.related_token_capture = &result;
                        auto added_function = program_structure.add_function_to_module(parent_module->full_path, func_def);
                        added_function->related_token_capture = &result;
                    } else if (parent->type_key == range_keys::struct_type_key) {
                        //it's a function/method in a struct!
                        auto struct_result = parent->get_captured_range(range_keys::struct_name);

                        if (struct_result) {
                            auto &struct_information = struct_result.value();
                            auto &original_struct_token = tokens[struct_information.from_token_index];
                            auto struct_full_path = parent_type->full_path;
                            auto full_function_name = struct_full_path + spelling::modules::module_seperator + function_name;

                            function_definition func_def = function_definition::create(full_function_name, true);
                            func_def.related_token_capture = &result;
                            auto added_function = program_structure.add_function_to_type(struct_full_path, func_def);
                            added_function->related_token_capture = &result;
                        }
                    }
                }
            }
        }
    }

    void parser::analyze_main_areas() {
        auto &ps = _context.get_program_structure();
        analyze_map(_code_map, _tokens, _context, nullptr, ps.get_global_module());
    }
} // shade
