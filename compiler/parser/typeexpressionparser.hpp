//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#ifndef SHADE_TYPEEXPRESSIONPARSER_HPP
#define SHADE_TYPEEXPRESSIONPARSER_HPP

#include "range_parser.hpp"

namespace shade {
    enum class type_expression_errors {
        core_type_not_in_expression,
        core_type_not_found_in_namespace,

    };

    class type_expression_parser {
        public:
            static std::expected<type_expression, type_expression_errors> parse(const std::vector<language_token> &tokens, size_t from_token_index, size_t to_token_index, compile_context &context, const cstring &current_namespace);
    };
}

#endif //SHADE_TYPEEXPRESSIONPARSER_HPP
