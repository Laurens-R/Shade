//
// Created by Laurens Ruijtenberg on 19/09/2026.
//

#include "conditional_parser.hpp"
#include "parser_range_keys.hpp"
#include "expressionparser.hpp"
#include "parse_methods.hpp"
#include "body_parser.hpp"

namespace shade {
    ast_node conditional_parser::parse_if(const std::vector<language_token> &tokens, const parsed_ranges &if_ranges, host_type host_environment_type, void *host_environment, compile_context &context) {
        ast_node if_node;
        ast_node condition_node;

        if (if_ranges.contains_key(range_keys::if_condition)) {
            auto condition_range = if_ranges.get_captured_range(range_keys::if_condition).value();
            condition_node = expression_parser::parse_expression(tokens, condition_range.from_token_index, condition_range.to_token_index, host_environment_type, host_environment, context);
            if_node.add_argument_node(condition_node);
        }

        if (if_ranges.contains_key(range_keys::if_body)) {
            auto & child_ranges = if_ranges.child_results;
            auto child_nodes = body_parser::parse_body(child_ranges, tokens, context, host_environment_type, &if_node);
            if_node.append_child_nodes(child_nodes);
        }

        return if_node;
    }
} // shade