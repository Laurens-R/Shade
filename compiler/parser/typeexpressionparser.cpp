//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#include "typeexpressionparser.hpp"
#include "../lang/spelling.hpp"
#include "parser_range_keys.hpp"
#include "parse_methods.hpp"

namespace shade {

    type_expression type_expression_parser::parse(const std::vector<language_token> &tokens, size_t from_token_index, size_t to_token_index, compile_context &context, const cstring &current_namespace) {
        type_expression result;
        auto & parsers = parse_methods::get_instance();

        auto parse_result = parsers.type_expressions.try_capture(tokens, from_token_index, to_token_index);

        if (parse_result.matched) {
            result.is_owning = parse_result.contains_key(range_keys::type_expression_owning);
            result.is_const = parse_result.contains_key(range_keys::type_expression_constant);
            result.is_ref = parse_result.contains_key(range_keys::type_expression_reference);
            result.is_ptr = parse_result.contains_key(range_keys::type_expression_pointer);
            result.is_array = parse_result.contains_key(range_keys::type_expression_array_range);

            auto & ps = context.get_program_structure();
        }

        return result;
    }
}
