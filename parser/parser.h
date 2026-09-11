//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_PARSER_H
#define SHADE_PARSER_H

#include <optional>
#include <vector>

#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"


namespace shade {
    class parser {
    public:
        static void try_scope_captures(const std::vector<language_token>& tokens, size_t from_token_index, size_t to_token_index);
        static void parse(const std::vector<language_token>& tokens, size_t from_token_index, size_t to_token_index);
    };

} // shade

#endif //SHADE_PARSER_H
