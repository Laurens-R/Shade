//
// Created by Laurens Ruijtenberg on 19/09/2026.
//

#ifndef SHADE_VARIABLE_DEFINTION_HPP
#define SHADE_VARIABLE_DEFINTION_HPP

#include "type_expression.hpp"

namespace shade {
    struct ast_node;

    struct variable_definition {
        cstring name = cstring::empty();
        type_expression type_declaration;
        size_t offset = 0;
        std::shared_ptr<ast_node> variable_expression;
    };
}

#endif //SHADE_VARIABLE_DEFINTION_HPP
