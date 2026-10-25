//
// Created by Laurens Ruijtenberg on 19/09/2026.
//

#ifndef SHADE_BODYPARSER_HPP
#define SHADE_BODYPARSER_HPP

#include <vector>

#include "host_type.hpp"
#include "range_parser.hpp"
#include "../ast/ast_node.hpp"

namespace shade {
    class body_parser {
        public:
            static std::vector<ast_node> parse_body(const std::vector<parsed_ranges> &body_ranges, const std::vector<language_token> &tokens, compile_context &context, host_type host_environment_type, void *host_environment);

    };
} // shade

#endif //SHADE_BODYPARSER_HPP
