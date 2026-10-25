//
// Created by Laurens Ruijtenberg on 19/09/2026.
//

#ifndef SHADE_IFPARSER_HPP
#define SHADE_IFPARSER_HPP

#include <compiler/ast/ast_node.hpp>

#include "range_parser.hpp"
#include "host_type.hpp"

namespace shade {
    class conditional_parser {
        public:
            ast_node parse_if(const std::vector<language_token> &tokens, const parsed_ranges & if_ranges, host_type host_environment_type, void * host_environment, compile_context &context);
    };
} // shade

#endif //SHADE_IFPARSER_HPP
