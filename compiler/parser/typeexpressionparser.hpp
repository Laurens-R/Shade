//
// Created by Laurens Ruijtenberg on 14/09/2026.
//

#ifndef SHADE_TYPEEXPRESSIONPARSER_HPP
#define SHADE_TYPEEXPRESSIONPARSER_HPP

#include "range_capture.hpp"

namespace shade {
    class type_expression_parser {
        range_capture _capture;
        public:
            type_expression_parser();

            type_expression parse(const captured_range & token_range, const std::vector<language_token> & tokens, compile_context & context, const cstring & current_namespace);
    };
}

#endif //SHADE_TYPEEXPRESSIONPARSER_HPP
