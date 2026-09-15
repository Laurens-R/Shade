//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#include "typeexpressionparser.hpp"
#include "../lang/spelling.hpp"
#include "parser_range_keys.hpp"

namespace shade {
    type_expression_parser::type_expression_parser() {
        //configure form [const] [ref/ptr] typename [array range] (where type is required)
        _capture.optional_match_and_capture_token(spelling::types::owning, range_keys::type_expression_owning);
        _capture.optional_match_and_capture_token(spelling::types::constant, range_keys::type_expression_constant);
        _capture.optional_match_and_capture_token(spelling::types::reference, range_keys::type_expression_reference);
        _capture.optional_match_and_capture_token(spelling::types::pointer, range_keys::type_expression_pointer);
        _capture.capture_tokens_by_count(1, range_keys::type_expression_typename);
        _capture.optional_capture_range(spelling::types::array_index_begin, spelling::types::array_index_end, range_keys::type_expression_array_range);
    }

    type_expression type_expression_parser::parse(const std::vector<language_token> &tokens, size_t from_token_index, size_t to_token_index, compile_context &context, const cstring &current_namespace) {
        type_expression result;

        auto capture_result = _capture.try_capture(tokens, from_token_index, to_token_index);

        if (capture_result.matched) {
            result.is_owning = capture_result.contains_key(range_keys::type_expression_owning);
            result.is_const = capture_result.contains_key(range_keys::type_expression_constant);
            result.is_ref = capture_result.contains_key(range_keys::type_expression_reference);
            result.is_ptr = capture_result.contains_key(range_keys::type_expression_pointer);
            result.is_array = capture_result.contains_key(range_keys::type_expression_array_range);

            auto & ps = context.get_program_structure();
        }

        return result;
    }
}
