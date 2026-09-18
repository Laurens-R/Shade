//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "expressionparser.hpp"
#include "typeexpressionparser.hpp"

#include "../lang/spelling.hpp"

namespace shade {
    ast_node expression_parser::parse_expression(const std::vector<language_token> &tokens, const parsed_range &expression_range, expression_parent_type parent_type, void *parent, compile_context &context) {
        ast_node result{};

        cstring parent_namespace;
        if (parent_type == expression_parent_type::function) {
            parent_namespace = static_cast<function_definition*>(parent)->full_path;
        } else if (parent_type == expression_parent_type::type) {
            parent_namespace = static_cast<type_definition*>(parent)->full_path;
        } else if (parent_type == expression_parent_type::module) {
            parent_namespace = static_cast<module_definition*>(parent)->full_path;
        }

        std::vector<size_t> assignment_indices;
        for (size_t i = expression_range.from_token_index; i <= expression_range.to_token_index; ++i) {
            if (tokens[i].text == spelling::variables::assignment) {
                assignment_indices.push_back(i);
            }
        }

        //next scan for a type expression (which is the case when defining a variable)
        auto found_type_expression = type_expression_parser::parse(tokens, expression_range.from_token_index, expression_range.to_token_index, context, parent_namespace);
        bool starts_with_type = found_type_expression.has_value();

        if (starts_with_type) {
            //there is a variable declaration here.
            auto type_expr = found_type_expression.value();
            size_t variable_declaration_token_index = type_expr.token_index_to + 1;

        }


        if (!assignment_indices.empty()) {

        }

        return result;
    }
}
