//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_RANGE_CAPTURE_H
#define SHADE_RANGE_CAPTURE_H

#include <optional>
#include <vector>
#include <unordered_map>

#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"

namespace shade {


    struct captured_range {
        cstring key;
        size_t  from_token_index = 0;
        size_t  to_token_index   = 0;
    };

    struct capture_results {
    private:
        std::vector<captured_range>             _captured_ranges;
        std::unordered_map<const char*, size_t> _captured_ranges_by_key;

    public:
        std::vector<capture_results> child_results;

        bool    matched = false;
        cstring failure_message;
        size_t  range_from_index = 0;
        size_t  range_to_index   = 0;

        void reset();

        void                          add_captured_range(size_t from, size_t to, const cstring& key);
        std::optional<captured_range> get_captured_range(const cstring& key);

    };

    class range_capture {
        enum class instruction_type {
            match_any,
            match_single,
            match_range,
            take_n,
            skip_until,
            skip_count,
            if_next_token_present,
            undefined
        };

        struct parse_instruction {
            instruction_type     type = instruction_type::undefined;
            std::vector<cstring> match_tokens;
            size_t               skip_count = 0;
            cstring              key;
        };

        std::vector<parse_instruction> _parse_instructions;

    public:
        range_capture& match_and_capture_token(const cstring& token, const cstring& key);
        range_capture& match_and_capture_any_token(const std::vector<cstring>& tokens, const cstring& key);
        range_capture& capture_tokens_by_count(size_t count, const cstring& key);
        range_capture& if_next_token_present(const cstring& token);
        range_capture& skip_tokens_by(size_t count);
        range_capture& skip_token_until(const cstring& token);
        range_capture& capture_range(const cstring& from, const cstring& to, const cstring& key);

        [[nodiscard]] capture_results try_capture(const std::vector<language_token>& tokens, size_t from_index, size_t to_index) const;

    };
}

#endif //SHADE_RANGE_CAPTURE_H
