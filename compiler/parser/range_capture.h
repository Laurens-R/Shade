//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_RANGE_CAPTURE_H
#define SHADE_RANGE_CAPTURE_H

#include <optional>
#include <vector>

#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"

namespace shade {

    enum class range_type {
        group,
        scope,
        undefined
    };

    struct token_range {
        size_t from_token_index = 0;
        size_t to_token_index = 0;
        cstring prelude = cstring::empty();
        range_type type = range_type::undefined;
    };

    struct captured_range {
        cstring top_level_prelude = cstring::empty();
        size_t from_token_index = 0;
        size_t to_token_index = 0;
        std::vector<token_range> ranges;
    };

    class range_capture {
        struct range_tokens {
            cstring from = cstring::empty();
            cstring to = cstring::empty();
            range_type type = range_type::undefined;
        };

        std::vector<cstring>      _captured;
        std::vector<range_tokens> _ranges;
        cstring                   _starts_with = cstring::empty();
        cstring                   _continue_until_token = cstring::empty();
    public:
        range_capture& if_start_with(const cstring & token);
        range_capture& then_capture_range(const cstring & from, const cstring & to, const range_type type);
        [[nodiscard]] std::optional<captured_range> try_capture(const std::vector<language_token>& tokens, size_t start_offset, size_t stop_index) const;

    };
}

#endif //SHADE_RANGE_CAPTURE_H
