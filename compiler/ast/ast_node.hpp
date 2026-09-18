//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_AST_NODE_H
#define SHADE_AST_NODE_H

#include <memory>
#include <optional>
#include <vector>


#include "../../shared/utils/cstring.hpp"
#include "../lexer/lexer.hpp"

namespace shade
{

    struct type_definition;
    struct function_definition;

    enum class ast_node_types {
        module_definition,
        function_definition,
        structure_definition,
        variable_definition,
        variable_reference,
        constant_value,
        constant_reference,
        operation,
        function_call,
        extern_function_call,
        extern_c_function_call
    };

    struct ast_node
    {
        ast_node * parent;
        ast_node_types type;
        std::vector<std::shared_ptr<ast_node>> children;
        language_token token;

        ast_node * create_child(ast_node * parent);
        std::optional<type_definition*> get_type_metadata(ast_node * parent);
        std::optional<function_definition*> get_function_metadata(ast_node * parent);
    };
} // shade

#endif //SHADE_AST_NODE_H
