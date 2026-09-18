//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#ifndef SHADE_EXPRESSIONPARSER_HPP
#define SHADE_EXPRESSIONPARSER_HPP

#include "../ast/ast_node.hpp"
#include "../compilecontext.hpp"
#include "range_parser.hpp"

namespace shade {
    enum class expression_parent_type {
        module,
        type,
        function
    };

    class expression_parser {
        public:
            static ast_node parse_expression(const std::vector<language_token> & tokens, const parsed_range & expression_range, expression_parent_type parent_type, void * parent, compile_context & context);
    };
}


#endif //SHADE_EXPRESSIONPARSER_HPP
