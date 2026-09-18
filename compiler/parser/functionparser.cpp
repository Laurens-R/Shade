//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "functionparser.hpp"
#include "typeexpressionparser.hpp"
#include "range_parser.hpp"
#include "parser_range_keys.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    bool function_parser::parse(const std::vector<language_token> &tokens, const parsed_ranges & ranges, compile_context &context, function_definition &func) {
        //first parse function arguments
        if (ranges.contains_key(range_keys::function_arguments)) {
            auto arguments_range = ranges.get_captured_range(range_keys::function_arguments).value();

            //first do a quick scan of the argument seperators, so we know how many arguments there are
            std::vector<size_t> found_argument_seperators;

            for (size_t argument_token_index = arguments_range.from_token_index; argument_token_index <= arguments_range.to_token_index; argument_token_index++) {
                if (tokens[argument_token_index].text == spelling::functions::func_argument_seperator) {
                    found_argument_seperators.push_back(argument_token_index);
                }
            }

            size_t argument_count = found_argument_seperators.size() + 1;

            for (size_t argument_index = 0; argument_index < argument_count; argument_index++) {
                cstring current_token = cstring::empty();
                cstring expected_end_token = argument_index == argument_count - 1 ? spelling::functions::end_arguments : spelling::functions::func_argument_seperator;
                size_t argument_token_start_index = argument_index == 0 ? arguments_range.from_token_index : found_argument_seperators[argument_index - 1] + 1;
                size_t argument_token_end_index = argument_index == argument_count - 1 ? arguments_range.to_token_index : (found_argument_seperators[argument_index] - 2); //-2 because if all is well, we would expect the symbol name of the argument at -1

                if (argument_token_end_index - argument_token_start_index < 1) {
                    //too few tokens for a valid argument
                    context.log_error(tokens[argument_token_start_index].line, tokens[argument_token_start_index].column, "Invalid argument. Make sure it contains at least a valid type and a name.");
                    return false;
                }

                auto argument_type_expression = type_expression_parser::parse(tokens, argument_token_start_index, argument_token_end_index, context, func.parent_path);
                if (argument_type_expression.has_value()) {
                    function_argument arg;
                    arg.type = argument_type_expression.value();
                    arg.name = tokens[argument_token_end_index + 1].text;
                    func.arguments.push_back(arg);
                } else {
                    context.log_error(tokens[argument_token_start_index].line, tokens[argument_token_start_index].column, "Invalid argument type. Could not determine the type for the argument.");
                    return false;
                }
            }
        }

        //parse the return type
        if (ranges.contains_key(range_keys::function_return_type)) {
            auto return_type_range = ranges.get_captured_range(range_keys::function_return_type).value();
            auto return_type = type_expression_parser::parse(tokens, return_type_range.from_token_index, return_type_range.to_token_index, context, func.parent_path);
            func.return_type = return_type.value();
            func.has_return_type = true;
        }

        return true;
    }
} // shade