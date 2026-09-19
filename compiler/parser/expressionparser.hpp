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
        function,
        expression
    };


    class expression_parser {
        private:
            static auto describe_operand(const ast_node &operand) -> cstring;
            static auto is_operand(const ast_node &node) -> bool;
            static bool is_right_associative(const ast_node &node);
            static bool is_operator(const ast_node &node);
            static uint8_t get_precedence(const ast_node &node);
            static ast_node build_expression_tree(const std::vector<ast_node> &nodes, compile_context &context);
            static variable_definition * find_variable(const cstring &name, module_definition *module, function_definition *function, compile_context &context);
            static function_definition * find_function(const cstring &name, module_definition *module, function_definition *function, compile_context &context);
        public:
            static ast_node parse_expression(const std::vector<language_token> &tokens, size_t expression_from_index, size_t expression_to_index, expression_parent_type parent_type, void *parent, compile_context &context);

    };
}


#endif //SHADE_EXPRESSIONPARSER_HPP
