//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#ifndef SHADE_TYPEEXPRESSIONPARSER_HPP
#define SHADE_TYPEEXPRESSIONPARSER_HPP

#include "range_parser.hpp"

namespace shade {
    class type_expression_parser {
        public:
            static type_expression parse(const std::vector<language_token> &tokens, size_t from_token_index, size_t to_token_index, compile_context &context, const cstring &current_namespace);
    };
}

#endif //SHADE_TYPEEXPRESSIONPARSER_HPP
