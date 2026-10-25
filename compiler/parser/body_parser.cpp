//
// Created by Laurens Ruijtenberg on 19/09/2026.
//

#include "body_parser.hpp"
#include "parser_range_keys.hpp"
#include "expressionparser.hpp"

namespace shade {
    std::vector<ast_node> body_parser::parse_body(const std::vector<parsed_ranges> &body_ranges, const std::vector<language_token> &tokens, compile_context &context, host_type host_environment_type, void * host_environment) {
        std::vector<ast_node> results;

        for (auto & range : body_ranges) {
            if (range.type_key == range_keys::statement_type_key) {
                ast_node node = expression_parser::parse_expression(tokens, range.range_from_index, range.range_to_index, host_environment_type, host_environment, context);
                results.push_back(node);
            }

            if (range.type_key == range_keys::if_type_key) {
                auto & child_ranges = range.child_results;
                auto child_nodes = body_parser::parse_body(child_ranges, tokens, context, host_environment_type, host_environment);
                results.append_range(child_nodes);
            }
        }

        return results;
    }
} // shade