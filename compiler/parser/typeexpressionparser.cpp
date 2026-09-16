//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#include "typeexpressionparser.hpp"
#include "../lang/spelling.hpp"
#include "parser_range_keys.hpp"
#include "parse_methods.hpp"
#include "../exceptions.hpp"

namespace shade {

    std::expected<type_expression, type_expression_errors> type_expression_parser::parse(const std::vector<language_token> &tokens, size_t from_token_index, size_t to_token_index, compile_context &context, const cstring &current_namespace) {
        type_expression result;
        auto & parsers = parse_methods::get_instance();

        auto parse_result = parsers.type_expressions.try_capture(tokens, from_token_index, to_token_index);

        if (parse_result.matched) {
            result.is_owned = parse_result.contains_key(range_keys::type_expression_owning);
            result.is_const = parse_result.contains_key(range_keys::type_expression_constant);
            result.is_ref = parse_result.contains_key(range_keys::type_expression_reference);
            result.is_ptr = parse_result.contains_key(range_keys::type_expression_pointer);
            result.is_array = parse_result.contains_key(range_keys::type_expression_fixed_array_range);
            result.is_dynamic_array = parse_result.contains_key(range_keys::type_expression_dynamic_array_range);

            if (result.is_array) {
                auto array_range = parse_result.get_captured_range(range_keys::type_expression_fixed_array_range);
                if (array_range) {
                    auto & array_range_result = array_range.value();

                    const auto& size_token = tokens[array_range_result.from_token_index + 1];
                    try {
                        result.fixed_array_size = size_token.text.to_u64();
                    } catch (...) {
                        context.log_error(size_token.line, size_token.column, "Invalid size specified for array.");
                    }
                }
            }

            auto & ps = context.get_program_structure();

            if (!parse_result.contains_key(range_keys::type_expression_typename)) return std::unexpected(type_expression_errors::core_type_not_in_expression);

            auto core_type_name = parse_result.get_captured_range(range_keys::type_expression_typename).value();
            auto core_type = ps.find_type_definition(tokens[core_type_name.from_token_index].text, current_namespace);


            if (!core_type) return std::unexpected(type_expression_errors::core_type_not_found_in_namespace);
            result.type = core_type;

        }

        return result;
    }
}
