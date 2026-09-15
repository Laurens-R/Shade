//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_PARSER_H
#define SHADE_PARSER_H

#include <optional>
#include <vector>

#include "range_capture.hpp"
#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"
#include "../compilecontext.hpp"

namespace shade {
    class parser {
        private:
            compile_context &_context;
            std::vector<capture_results> _code_map;
            const std::vector<language_token> &_tokens;

        public:
            parser(compile_context &context, const std::vector<language_token> &tokens);

            std::vector<capture_results> &capture_main_areas();
            void analyze_main_areas();


    };
} // shade

#endif //SHADE_PARSER_H
