//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_PARSER_H
#define SHADE_PARSER_H

#include <optional>
#include <vector>

#include "range_capture.h"
#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"
#include "../pipeline/compilecontext.h"

namespace shade {
    class parser {
    private:
        compile_context & _context;
        std::vector<capture_results> _code_map;
    public:
        parser(compile_context & context);

        std::vector<capture_results> & first_pass(const std::vector<language_token>& tokens, size_t from_token_index, size_t to_token_index);
        void analyze_first_pass();
    };

} // shade

#endif //SHADE_PARSER_H
