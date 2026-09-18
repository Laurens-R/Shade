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
        private:
            static auto describe_operand(const ast_node &operand) -> cstring;
            static auto is_operand(const ast_node *node) -> bool;
            static bool is_right_associative(const ast_node *node);
            static bool is_operator(const ast_node *node);
            static uint8_t get_precedence(const ast_node *node);
            static ast_node build_expression_tree(const std::vector<std::shared_ptr<ast_node> > &nodes, const parsed_range &expression_range, expression_parent_type parent_type, void *parent, compile_context &context);
        public:
            static std::shared_ptr<ast_node> parse_expression(const std::vector<language_token> &tokens, const parsed_range &expression_range, expression_parent_type parent_type, void *parent, compile_context &context);

    };
}


#endif //SHADE_EXPRESSIONPARSER_HPP
