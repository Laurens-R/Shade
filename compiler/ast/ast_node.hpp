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
        binary_logic,
        assignment,
        conditional,
        constant_reference,
        constant_value,
        extern_c_function_call,
        extern_function_call,
        function_call,
        function_definition,
        module_definition,
        operation,
        return_statement,
        structure_definition,
        variable_definition,
        variable_reference,
        break_statement,
        exit_statement,
        continue_statement,
        initializer_list,
        alloc,
        array_alloc,
        free_statement,
        sizeof_statement,
        length_of_statement,
        begin_precedence,
        end_precedence,
        string_subscript
    };

    struct ast_node
    {
        ast_node * parent;
        ast_node_types type;
        std::vector<ast_node> children;
        language_token token;
        cstring value;
        bool is_unary_operation;
        bool is_binary_node;

        void create_child(ast_node_types type);

        void set_left_node(const ast_node &node);
        void set_right_node(const ast_node &node);

        ast_node *get_left_node();

        ast_node *get_right_node();
    };
} // shade

#endif //SHADE_AST_NODE_H
