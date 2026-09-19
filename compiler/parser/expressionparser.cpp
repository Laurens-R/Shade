//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "expressionparser.hpp"

#include <format>

#include "parse_methods.hpp"
#include "typeexpressionparser.hpp"
#include "symbol_utils.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    auto expression_parser::describe_operand(const ast_node &operand) -> cstring {
        if (operand.type == ast_node_types::end_precedence) return "the parenthesised value";
        if (operand.type == ast_node_types::initializer_list) return "`{...}`";
        if (!operand.value.is_empty()) {
            return cstring("`") + operand.value + cstring("`");
        }

        return "a value";
    }

    auto expression_parser::is_operand(const ast_node &node) -> bool {
        switch (node.type) {
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

    bool expression_parser::is_right_associative(const ast_node &node) {
        return node.type == ast_node_types::assignment
               || node.type == ast_node_types::return_statement
               || node.type == ast_node_types::conditional
               || node.is_unary_operation;
    }

    bool expression_parser::is_operator(const ast_node &node) {
        return node.type == ast_node_types::operation
               || node.type == ast_node_types::assignment
               || node.type == ast_node_types::binary_logic
               || node.type == ast_node_types::return_statement
               || node.type == ast_node_types::conditional;
    }

    uint8_t expression_parser::get_precedence(const ast_node &node) {
        if (node.is_unary_operation) return 12;
        if (node.type == ast_node_types::assignment) return 1;
        if (node.type == ast_node_types::return_statement) return 1;
        if (node.type == ast_node_types::conditional) return 2;
        if (node.token->text == spelling::logic::logic_or) return 3;
        if (node.token->text == spelling::logic::logic_and) return 4;
        if (node.token->text == spelling::operators::operator_bitwise_or) return 5;
        if (node.token->text == spelling::operators::operator_bitwise_xor) return 6;
        if (node.token->text == spelling::operators::operator_bitwise_and) return 7;
        if (node.token->text == spelling::comparisons::comparison_equal
            || node.token->text == spelling::comparisons::comparison_not_equal
            || node.token->text == spelling::comparisons::comparison_less
            || node.token->text == spelling::comparisons::comparison_greater
            || node.token->text == spelling::comparisons::comparison_less_equal
            || node.token->text == spelling::comparisons::comparison_greater_equal)
            return 8;
        if (node.token->text == spelling::operators::operator_lshift
            || node.token->text == spelling::operators::operator_rshift)
            return 9;
        if (node.token->text == spelling::operators::operator_add
            || node.token->text == spelling::operators::operator_sub)
            return 10;
        if (node.token->text == spelling::operators::operator_mul
            || node.token->text == spelling::operators::operator_div
            || node.token->text == spelling::operators::operator_mod)
            return 11;
        return 0;
    }

    ast_node expression_parser::build_expression_tree(const std::vector<ast_node> &nodes, compile_context &context) {
        std::vector<ast_node> output_queue;
        std::vector<ast_node> operator_stack;

        // Shunting Yard: Convert infix to postfix
        for (const auto &node: nodes) {
            //an operand goes straight to the output queue. A POSTFIX string subscript is one too: it
            //binds tighter than every operator on the stack, so its operand is already the last thing in
            //the queue and nothing needs popping first.
            if (is_operator(node)) {
                output_queue.push_back(node);
            } else if (node.type == ast_node_types::begin_precedence) {
                operator_stack.push_back(node);
            } else if (node.type == ast_node_types::end_precedence) {
                //pop operators back to the matching '(' then discard the '('.
                while (!operator_stack.empty() &&
                       operator_stack.back().type != ast_node_types::begin_precedence) {
                    output_queue.push_back(operator_stack.back());
                    operator_stack.pop_back();
                }
                if (!operator_stack.empty()) {
                    operator_stack.pop_back();
                }
            } else if (is_operator(node)) {
                while (!operator_stack.empty() &&
                       is_operator(operator_stack.back())
                       && (get_precedence(operator_stack.back()) > get_precedence(node)
                           || (get_precedence(operator_stack.back()) == get_precedence(node)
                               && !is_right_associative(node)))) {
                    output_queue.push_back(operator_stack.back());
                    operator_stack.pop_back();
                }

                operator_stack.push_back(node);
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
        for (auto &token: output_queue) {
            if (token.type == ast_node_types::string_subscript) {
                //a POSTFIX operator: it takes the ONE value to its left. With no value there the source
                //wrote a subscript where nothing precedes it, which is a malformed expression rather
                //than a subscript of anything.
                if (eval_stack.empty()) {
                    context.log_error(token.token->line, token.token->column, "Subscript [...] has no value to its left.");
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
                    context.log_error(token.token->line, token.token->column, "A ? conditional has no condition to its left.");
                }
                ast_node conditional_node = token;
                conditional_node.set_left_node(eval_stack.back());
                eval_stack.pop_back();
                eval_stack.push_back(conditional_node);
            } else if (is_operator(token)) {
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
            context.log_error(nodes.front().token->line, nodes.front().token->column, cstring(std::format(
                                  "{} and {} are two values with nothing joining them — an operator or a `,` separator "
                                  "is missing.", describe_operand(eval_stack[0]).to_c_string(),
                                  describe_operand(eval_stack[1]).to_c_string())));
        }

        if (eval_stack.empty()) {
            context.log_error(nodes.front().token->line, nodes.front().token->column, "Expression is empty");
        }

        return eval_stack.front();
    }

    variable_definition * expression_parser::find_variable(const cstring & name, module_definition *module, function_definition *function, compile_context & context) {
        //TODO: this needs to deal with relative paths.
        if (module) {
            return module->find_variable(name);
        }

        if (function) {
            auto found_variable = function->find_variable(name);

            if (!found_variable && function->is_method) {
                auto parent_type = context.get_program_structure().get_type_definition(function->parent_path);
                if (parent_type) {
                    return parent_type->find_field(name);
                }
                return nullptr;
            }

            return found_variable;
        }

        return nullptr;
    }

    function_definition * expression_parser::find_function(const cstring &name, module_definition *module, function_definition *function, compile_context &context) {
        if (module) {
            return module->find_function_by_relative_path(name);
        }

        if (function) {
            if (function->is_method) {
                auto parent_type = context.get_program_structure().get_type_definition(function->parent_path);
                auto found_method = parent_type->find_method(name);
                if (!found_method) {
                    auto parent_module = context.get_program_structure().get_module_definition(function->parent_path);
                    return parent_module->find_function_by_relative_path(name);
                }

                return found_method;

            } else {
                auto parent_module = context.get_program_structure().get_module_definition(function->parent_path);
                return parent_module->find_function_by_relative_path(name);
            }
        }

        return nullptr;
    }

    ast_node expression_parser::parse_expression(const std::vector<language_token> &tokens, const size_t expression_from_index, const size_t expression_to_index, expression_parent_type parent_type, void *parent, compile_context &context) {
        ast_node result;
        std::vector<ast_node> flat_generated_nodes;

        function_definition *parent_function = nullptr;
        module_definition *parent_module = nullptr;
        size_t current_token_index = expression_from_index;
        size_t last_token_index = expression_to_index;

        cstring parent_namespace;

        //check for the parent type. Expressions cannot be part of a type/struct defintion.
        if (parent_type == expression_parent_type::function) {
            parent_function = static_cast<function_definition *>(parent);
            parent_namespace = parent_function->full_path;
        } else if (parent_type == expression_parent_type::module) {
            parent_module = static_cast<module_definition *>(parent);
            parent_namespace = parent_module->full_path;
        } else if (parent_type == expression_parent_type::type) {
            context.log_error(tokens[expression_from_index].line, tokens[expression_from_index].column, "Statements are not allowed within a type definition: use a method instead.");
            return result;
        }

        //check if we are part of a sub expression and sub expression can't have assignments or variable defintions.
        if (parent_type != expression_parent_type::expression) {
            //first scan for assignment symbols.
            std::vector<size_t> assignment_indices;

            for (size_t i = expression_from_index; i <= expression_to_index; ++i) {
                if (tokens[i].text == spelling::variables::assignment) {
                    assignment_indices.push_back(i);
                }
            }

            //we have found an assignment. We must resolve if it is a declaration or an existing variable and add the corresponding
            //nodes;
            if (assignment_indices.size() == 1) {
                ast_node variable_node;
                variable_node.type = ast_node_types::variable_reference;
                ast_node assignment_node;
                assignment_node.type = ast_node_types::assignment;

                size_t assignment_index = assignment_indices[0];
                if (assignment_index == expression_from_index) {
                    context.log_error(tokens[assignment_indices[0]].line, tokens[assignment_indices[0]].column, "Assignment at start of expression. An assignment must always point to a variable or field.");
                    return result;
                }

                //either we create a new one in here in case we are dealing with a new variable declaration
                //or we put in the existing variable.
                variable_definition variable;

                auto found_type_expression = type_expression_parser::parse(tokens, expression_from_index, expression_to_index, context, parent_namespace);
                bool starts_with_type = found_type_expression.has_value();

                if (starts_with_type) {
                    //there is a variable declaration here. We need to add a new one.
                    auto type_expr = found_type_expression.value();
                    size_t variable_declaration_token_index = type_expr.token_index_to + 1;
                    auto variable_name = tokens[variable_declaration_token_index].text;
                    bool is_valid_name = symbol_utils::is_valid_identifier(variable_name);

                    if (!is_valid_name) {
                        context.log_error(tokens[variable_declaration_token_index].line, tokens[variable_declaration_token_index].column, "Invalid variable name");
                        return result;
                    }

                    variable_definition variable;
                    variable.name = variable_name;
                    variable.type_declaration = type_expr;

                    if (parent_function) {
                        parent_function->add_variable(variable);
                    } else if (parent_module) {
                        parent_module->add_variable(variable);
                    }

                    variable_node.token = const_cast<language_token *>(&tokens[variable_declaration_token_index]);
                    variable_node.value = variable_name;
                } else {
                    //we are putting something into an existing variable, we need to check if it exists and load the information

                    //if no type is defined we expect the variable name is located at the very beginning of the expression
                    auto variable_name = tokens[expression_from_index].text;
                    variable_node.token = const_cast<language_token *>(&tokens[expression_from_index]);
                    variable_node.value = variable_name;
                    const std::vector<variable_definition> *variables = nullptr;

                    if (parent_function) {
                        variables = &parent_function->variables;
                    } else if (parent_module) {
                        variables = &parent_module->variables;
                    }

                    variable_definition *found_definition = find_variable(variable_name, parent_module, parent_function, context);

                    if (!found_definition) {
                        context.log_error(tokens[expression_from_index].line, tokens[expression_from_index].column, "Variable not found. Make sure to declare it at a valid scope.");
                        return result;
                    }
                }

                flat_generated_nodes.push_back(variable_node);
                flat_generated_nodes.push_back(assignment_node);

                current_token_index = assignment_index + 1;
            } else if (assignment_indices.size() > 1) {
                context.log_error(tokens[expression_from_index].line, tokens[expression_to_index].column, "Multiple assignments in expression");
            }
        }

        bool in_string = false;
        cstring current_string_value;

        //next we are going to walk through the operations side of the expression.
        while (current_token_index <= last_token_index) {
            auto & current_token = tokens[current_token_index];

            if (current_token.text == spelling::literals::literal_string_indicator) {
                in_string = !in_string;
                current_token_index++;
                continue;
            }

            if (in_string) {
                current_string_value = current_token.text;
                ast_node node;
                node.type = ast_node_types::constant_value;
                node.value = current_string_value;
                node.token = const_cast<language_token*>(&current_token);
                node.related_type = context.get_program_structure().get_type_definition(spelling::types::keyword_string);
                flat_generated_nodes.push_back(node);
                current_token_index++;
                continue;
            }

            if (symbol_utils::is_primitive_literal(current_token.text)) {
                auto literal_typename = symbol_utils::get_approximate_literal_typename(current_token.text);

                ast_node node;
                node.type = ast_node_types::constant_value;
                node.value = current_token.text;
                node.token = const_cast<language_token*>(&current_token);
                node.related_type = context.get_program_structure().get_type_definition(literal_typename);
                flat_generated_nodes.push_back(node);
                current_token_index++;
                continue;
            }

            if (symbol_utils::is_operator(current_token.text)) {
                ast_node node;
                node.type = ast_node_types::operation;
                node.value = current_token.text;
                node.token = const_cast<language_token*>(&current_token);
                flat_generated_nodes.push_back(node);
                current_token_index++;
                continue;
            }

            //if it's not operations or literals we go look for a variable
            auto found_variable = find_variable(current_token.text, parent_module, parent_function, context);

            if (found_variable) {
                ast_node node;
                node.type = ast_node_types::variable_reference;
                node.token = const_cast<language_token*>(&current_token);
                node.related_type_expression = &found_variable->type_declaration;
                node.related_type = found_variable->type_declaration.type;
                node.value = found_variable->name;
                flat_generated_nodes.push_back(node);
                current_token_index++;
                continue;
            }

            auto found_function = find_function(current_token.text, parent_module, parent_function, context);

            if (found_function) {
                auto & argument_parser = parse_methods::get_instance().function_arguments;
                auto argument_capture_result = argument_parser.try_capture(tokens, current_token_index + 1, last_token_index);

                ast_node node;
                node.type = ast_node_types::function_call;
                node.token = const_cast<language_token*>(&current_token);
                node.related_function_definition = found_function;
                node.related_type_expression = &found_function->return_type;
                node.related_type = !found_function->return_type.is_void() ? found_function->return_type.type : nullptr;
                node.value = found_function->name;

                //there are function call arguments. (it is also fine if there are none, in case a function is passed as a lambda)
                if (argument_capture_result.matched) {
                    size_t from_argument_index = argument_capture_result.range_from_index;
                    size_t to_argument_index = argument_capture_result.range_to_index;
                    size_t current_argument_token_index = from_argument_index;

                    //parse through all the sub-expressions for each argument and recursively call this parse method to parse them individually.
                    while (current_argument_token_index <= to_argument_index) {
                        auto & current_argument_token = tokens[current_argument_token_index].text;
                        if (current_argument_token == spelling::functions::func_argument_seperator || current_argument_token == spelling::functions::end_arguments) {
                            ast_node argument_node = parse_expression(tokens, from_argument_index, to_argument_index, expression_parent_type::expression, nullptr, context);
                            node.children.push_back(argument_node);
                            from_argument_index = current_argument_token_index + 1;
                        }

                        current_argument_token_index++;
                    }
                }

                flat_generated_nodes.push_back(node);
                current_token_index++;
                continue;
            }
        }

        //after we've parsed the entire token scope for the expression, we apply shunting yard to it:
        result = build_expression_tree(flat_generated_nodes, context);


        return result;
    }
}
