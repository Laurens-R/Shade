//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_PARSER_H
#define SHADE_PARSER_H

#include <optional>
#include <vector>

#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"
#include "../pipeline/compilecontext.h"

namespace shade {
    class parser {
    private:
        compile_context & _context;
    public:
        parser(compile_context & context);

        void try_scope_captures(const std::vector<language_token>& tokens, size_t from_token_index, size_t to_token_index);
        void parse(const std::vector<language_token>& tokens, size_t from_token_index, size_t to_token_index);
    };

} // shade

#endif //SHADE_PARSER_H
