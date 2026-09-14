//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#include "typeexpressionparser.hpp"

namespace shade {

    type_expression_parser::type_expression_parser() {
        //configure form [const] [ref/ptr] typename [array range] (where type is required)
        //todo: replace keys and tokens with constexpr variations/spelling definitions
        _capture.optional_match_and_capture_token("owned", "owned_key");
        _capture.optional_match_and_capture_token("const", "const_key");
        _capture.optional_match_and_capture_token("ref", "ref_key");
        _capture.optional_match_and_capture_token("ptr", "ptr_key");
        _capture.capture_tokens_by_count(1, "typename");
        _capture.optional_capture_range("[", "]", "array range");
    }

    type_expression type_expression_parser::parse(const captured_range &token_range, const std::vector<language_token> &tokens, compile_context & context, const cstring & current_namespace) {
        type_expression result;

        //note: I expect we only have to pass the namespace for the module. types are only known at the global or nested module level. You can't have seperate types in
        //structs/types.

        return result;
    }
}
