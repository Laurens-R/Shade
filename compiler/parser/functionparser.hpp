//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#ifndef SHADE_FUNCTIONPARSER_HPP
#define SHADE_FUNCTIONPARSER_HPP

#include <shared/utils/cstring.hpp>
#include "../lexer/lexer.hpp"
#include "../compilecontext.hpp"
#include "../meta/function_definition.hpp"
#include "range_parser.hpp"

namespace shade {
    class function_parser {
        private:
            static bool validate_function_name(const std::vector<language_token> &tokens, const parsed_ranges &ranges, compile_context &context, function_definition &func);
        public:
            static bool parse(const std::vector<language_token> &tokens, const parsed_ranges & ranges, compile_context &context, function_definition & func);
    };
} // shade

#endif //SHADE_FUNCTIONPARSER_HPP
