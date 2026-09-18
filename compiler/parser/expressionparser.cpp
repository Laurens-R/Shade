//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "expressionparser.hpp"

#include <format>

#include "typeexpressionparser.hpp"
#include "symbols.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    auto expression_parser::describe_operand(const ast_node& operand) -> cstring {
        if (operand.type == ast_node_types::end_precedence) return "the parenthesised value";
        if (operand.type == ast_node_types::initializer_list) return "`{...}`";
        if (!operand.value.is_empty()) {
            return cstring("`") + operand.value + cstring("`");
        }

        return "a value";
    }

    auto expression_parser::is_operand(const ast_node * node) -> bool {
        switch (node->type) {
            case ast_node_types::constant_value:
            case ast_node_types::variable_reference:
            case ast_node_types::function_call:
            case ast_node_types::extern_function_call:
            case ast_node_types::extern_c_function_call:
            case ast_node_types::array_alloc:
            case ast_node_types::initializer_list:
            case ast_node_types::break_statement:
            case ast_node_types::exit_statement:
            case ast_node_types::continue_statement:
            case ast_node_types::free_statement:
            case ast_node_types::sizeof_statement:
            case ast_node_types::length_of_statement:
                return true;
                //default-is-safe: a MEMBERSHIP test — every enumerator not named is outside the set by definition, so a new kind is refused, never admitted
            default:
                return false;
        }
    }

    bool expression_parser::is_right_associative(const ast_node * node) {
        return node->type == ast_node_types::assignment
               || node->type == ast_node_types::return_statement
               || node->type == ast_node_types::conditional
               || node->is_unary_operation;
    }

    bool expression_parser::is_operator(const ast_node * node) {
        return node->type == ast_node_types::operation
               || node->type == ast_node_types::assignment
               || node->type == ast_node_types::binary_logic
               || node->type == ast_node_types::return_statement
               || node->type == ast_node_types::conditional;
    }

    uint8_t expression_parser::get_precedence(const ast_node * node) {
        if (node->is_unary_operation) return 12;
        if (node->type == ast_node_types::assignment) return 1;
        if (node->type == ast_node_types::return_statement) return 1;
        if (node->type == ast_node_types::conditional) return 2;
        if (node->token.text == spelling::logic::logic_or) return 3;
        if (node->token.text == spelling::logic::logic_and) return 4;
        if (node->token.text == spelling::operators::operator_bitwise_or) return 5;
        if (node->token.text == spelling::operators::operator_bitwise_xor) return 6;
        if (node->token.text == spelling::operators::operator_bitwise_and) return 7;
        if (node->token.text == spelling::comparisons::comparison_equal
            || node->token.text == spelling::comparisons::comparison_not_equal
            || node->token.text == spelling::comparisons::comparison_less
            || node->token.text == spelling::comparisons::comparison_greater
            || node->token.text == spelling::comparisons::comparison_less_equal
            || node->token.text == spelling::comparisons::comparison_greater_equal)
            return 8;
        if (node->token.text == spelling::operators::operator_lshift
            || node->token.text == spelling::operators::operator_rshift)
            return 9;
        if (node->token.text == spelling::operators::operator_add
            || node->token.text == spelling::operators::operator_sub)
            return 10;
        if (node->token.text == spelling::operators::operator_mul
            || node->token.text == spelling::operators::operator_div
            || node->token.text == spelling::operators::operator_mod)
            return 11;
        return 0;
    }

    ast_node expression_parser::build_expression_tree(const std::vector<std::shared_ptr<ast_node> > &nodes, const parsed_range &expression_range, expression_parent_type parent_type, void *parent, compile_context &context) {
        std::vector<ast_node> output_queue;
        std::vector<ast_node> operator_stack;

        // Shunting Yard: Convert infix to postfix
        for (const auto& node : nodes) {
            //an operand goes straight to the output queue. A POSTFIX string subscript is one too: it
            //binds tighter than every operator on the stack, so its operand is already the last thing in
            //the queue and nothing needs popping first.
            if (is_operator(node.get())) {
                output_queue.push_back(*node);
            } else if (node->type == ast_node_types::begin_precedence) {
                operator_stack.push_back(*node);
            } else if (node->type == ast_node_types::end_precedence) {
                //pop operators back to the matching '(' then discard the '('.
                while (!operator_stack.empty() &&
                    operator_stack.back().type != ast_node_types::begin_precedence) {
                        output_queue.push_back(operator_stack.back());
                        operator_stack.pop_back();
                    }
                if (!operator_stack.empty()) {
                    operator_stack.pop_back();
                }
            } else if (is_operator(node.get())) {
                while (!operator_stack.empty() &&
                    is_operator(&operator_stack.back())
                    && (get_precedence(&operator_stack.back()) > get_precedence(node.get())
                        || (get_precedence(&operator_stack.back()) == get_precedence(node.get())
                            && !is_right_associative(node.get())))) {
                                    output_queue.push_back(operator_stack.back());
                                    operator_stack.pop_back();
                                }

                operator_stack.push_back(*node);
            }
        }

        while (!operator_stack.empty()) {
            //a leftover '(' would mean unbalanced parentheses; skip it defensively.
            if (operator_stack.back().type != ast_node_types::begin_precedence) {
                output_queue.push_back(operator_stack.back());
            }
            operator_stack.pop_back();
        }

        // Build tree from postfix expression
        std::vector<ast_node> eval_stack;
        for (auto& token : output_queue) {
            if (token.type == ast_node_types::string_subscript) {
                //a POSTFIX operator: it takes the ONE value to its left. With no value there the source
                //wrote a subscript where nothing precedes it, which is a malformed expression rather
                //than a subscript of anything.
                if (eval_stack.empty()) {
                    context.log_error(token.token.line, token.token.column, "Subscript [...] has no value to its left.");
                }
                ast_node subscript_node = token;
                subscript_node.set_left_node(eval_stack.back());
                eval_stack.pop_back();
                eval_stack.push_back(subscript_node);
            } else if (token.type == ast_node_types::conditional) {
                //the conditional binds exactly ONE operand here — its CONDITION, the value to its left.
                //Its two arms were parsed whole at the `?` (each is a complete sub-expression), so they
                //are already on the node and never pass through the yard: that is what makes the arms
                //independent of the operators around the conditional, and what makes exactly one of them
                //evaluable at run time.
                if (eval_stack.empty()) {
                    context.log_error(token.token.line, token.token.column, "A ? conditional has no condition to its left.");
                }
                ast_node conditional_node = token;
                conditional_node.set_left_node(eval_stack.back());
                eval_stack.pop_back();
                eval_stack.push_back(conditional_node);
            } else if (is_operator(&token)) {
                ast_node op_node = token;

                if (token.is_unary_operation) {
                    // prefix unary (!, ~): a single operand to the right.
                    if (!eval_stack.empty()) {
                        auto operand = eval_stack.back();
                        eval_stack.pop_back();
                        op_node.set_right_node(operand);
                    }
                } else if (eval_stack.size() >= 2) {
                    auto right = eval_stack.back();
                    eval_stack.pop_back();
                    auto left = eval_stack.back();
                    eval_stack.pop_back();
                    op_node.set_left_node(left);
                    op_node.set_right_node(right);
                } else if (eval_stack.size() == 1) {
                    // Unary operator (e.g., return)
                    auto operand = eval_stack.back();
                    eval_stack.pop_back();
                    op_node.set_right_node(operand);
                }

                eval_stack.push_back(op_node);
            } else {
                eval_stack.push_back(token);
            }
        }

        //A well-formed expression reduces to exactly ONE node. More than one means two operands stood side
        //by side with nothing joining them, and taking `evalStack[0]` silently threw the others away —
        //which is what every "a missing `,` was accepted" case in the language actually was. See the
        //header: `{1 2}`, `h(1 2)` and `g[1 1]` all reached here with two operands and no operator.
        if (eval_stack.size() > 1) {
            context.log_error(nodes.front()->token.line, nodes.front()->token.column, cstring(std::format(
                "{} and {} are two values with nothing joining them — an operator or a `,` separator "
                "is missing.", describe_operand(eval_stack[0]).to_c_string(),
                describe_operand(eval_stack[1]).to_c_string())));
        }

        if (eval_stack.empty()) {
            context.log_error(nodes.front()->token.line, nodes.front()->token.column, "Expression is empty");
        }

        return eval_stack.front();
    }

    std::shared_ptr<ast_node> expression_parser::parse_expression(const std::vector<language_token> &tokens, const parsed_range &expression_range, expression_parent_type parent_type, void *parent, compile_context &context) {
        std::shared_ptr<ast_node> result = std::make_shared<ast_node>();

        cstring parent_namespace;
        if (parent_type == expression_parent_type::function) {
            parent_namespace = static_cast<function_definition *>(parent)->full_path;
        } else if (parent_type == expression_parent_type::type) {
            parent_namespace = static_cast<type_definition *>(parent)->full_path;
        } else if (parent_type == expression_parent_type::module) {
            parent_namespace = static_cast<module_definition *>(parent)->full_path;
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
            auto variable_name = tokens[variable_declaration_token_index].text;
            bool is_valid_name = symbols::is_valid_identifier(variable_name);
            if (!is_valid_name) {
                context.log_error(tokens[variable_declaration_token_index].line, tokens[variable_declaration_token_index].column, "Invalid variable name");
                return result;
            }
        }


        if (!assignment_indices.empty()) {}

        return result;
    }
}
