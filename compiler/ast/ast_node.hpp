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

namespace shade {

    struct type_definition;

#ifndef SHADE_FUNCTION_DEFINITION_DEFINED
    struct function_definition;
#endif

    enum class ast_node_types {
        alloc,
        array_alloc,
        assignment,
        begin_precedence,
        binary_logic,
        break_statement,
        conditional,
        constant_reference,
        constant_value,
        continue_statement,
        empty,
        end_precedence,
        exit_statement,
        extern_c_function_call,
        extern_function_call,
        free_statement,
        function_call,
        function_definition,
        initializer_list,
        length_of_statement,
        module_definition,
        operation,
        return_statement,
        sizeof_statement,
        string_subscript,
        structure_definition,
        unknown,
        variable_definition,
        variable_reference,
    };

    struct ast_node {
        private:
            std::shared_ptr<std::vector<ast_node> > children;
            std::shared_ptr<std::vector<ast_node> > argument_nodes;

        public:
            ast_node *parent = nullptr;
            ast_node_types type = ast_node_types::unknown;
            language_token *token = nullptr;
            type_definition *related_type = nullptr;
            type_expression *related_type_expression = nullptr;
            function_definition *related_function_definition = nullptr;
            cstring value;
            bool is_unary_operation = false;
            bool is_binary_node = false;

            bool has_arguments() const;

            bool has_child_nodes() const;

            void ensure_child_nodes();

            void ensure_argument_nodes();

            void set_left_node(const ast_node &node);

            void set_right_node(const ast_node &node);

            ast_node *get_left_node();

            ast_node *get_right_node();

            void append_child_nodes(const std::vector<ast_node> &nodes);

            void add_child_node(const ast_node &node);

            [[nodiscard]] ast_node &get_child_node(size_t index) const;

            [[nodiscard]] size_t get_child_node_count() const;

            void add_argument_node(const ast_node &node);

            [[nodiscard]] ast_node &get_argument_node(size_t index) const;

            [[nodiscard]] size_t get_argument_node_count() const;
    };
} // shade

#endif //SHADE_AST_NODE_H
