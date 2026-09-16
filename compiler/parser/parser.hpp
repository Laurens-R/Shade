//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_PARSER_H
#define SHADE_PARSER_H

#include <optional>
#include <vector>

#include "range_parser.hpp"
#include "../../shared/utils/cstring.hpp"
#include "../lexer/lexer.hpp"
#include "../compilecontext.hpp"

namespace shade {
    class parser {
        private:
            compile_context &_context;
            std::vector<parsed_ranges> _code_map;
            const std::vector<language_token> &_tokens;

        public:
            parser(compile_context &context, const std::vector<language_token> &tokens);

            std::vector<parsed_ranges> &capture_main_areas();
            void analyze_main_areas();


    };
} // shade

#endif //SHADE_PARSER_H
