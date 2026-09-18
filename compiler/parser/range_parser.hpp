//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_RANGE_CAPTURE_H
#define SHADE_RANGE_CAPTURE_H

#include <optional>
#include <vector>
#include <unordered_map>

#include "../../shared/utils/cstring.hpp"
#include "../lexer/lexer.hpp"

namespace shade {
    struct parsed_range {
        cstring key;
        size_t from_token_index = 0;
        size_t to_token_index = 0;
    };

    struct parsed_ranges {
        private:
            std::vector<parsed_range> _captured_ranges;
            std::unordered_map<std::string_view, size_t> _captured_ranges_by_key;

        public:
            std::vector<parsed_ranges> child_results;

            cstring failure_message;
            cstring type_key;
            size_t range_from_index = 0;
            size_t range_to_index = 0;
            bool matched = true;

            void reset();

            void add_captured_range(size_t from, size_t to, const cstring &key);

            std::optional<parsed_range> get_captured_range(const cstring &key) const;

            bool contains_key(const cstring &key) const;

            size_t get_size() const;

            std::optional<parsed_range> get_captured_range_at(const size_t index) const;
    };

    /**
     * A parser utility that captures ranges of tokens based on instructions and which can
     * store parsed ranges under a key. This helps in saving a lot of manual token loops.
     */
    class range_parser {
        enum class instruction_type {
            match_any,
            match_single,
            match_range,
            take_n,
            take_until,
            skip_until,
            skip_count,
            if_next_token_present,
            optional_match_single,
            optional_match_range,
            optional_take_until_if_previous_key,
            undefined
        };

        struct parse_instruction {
            instruction_type type = instruction_type::undefined;
            std::vector<cstring> match_tokens;
            size_t skip_count = 0;
            cstring key;
            cstring previous_key;
        };

        cstring capture_type_key;
        std::vector<parse_instruction> _parse_instructions;

        public:
            /**
             * Check if an optional token is present and capture it under the given key. Otherwise proceed to the next token.
             */
            range_parser &optional_match_and_capture_token(const cstring &token, const cstring &key);

            /**
             * Check if a token is present and capture it under the given key. The try_capture method will return early if it fails and it's result will have the matches field set to false.
             */
            range_parser &match_and_capture_token(const cstring &token, const cstring &key);

            /**
             * Check if any of the given tokens is present and capture it under the given key. The try_capture method will return early if it fails and it's result will have the matches field set to false.
             */
            range_parser &match_and_capture_any_token(const std::vector<cstring> &tokens, const cstring &key);

            /**
             * Simply take the next tokens by count and capture them under the given key.
             */
            range_parser &capture_tokens_by_count(size_t count, const cstring &key);

            /**
             * A conditional requirement that will return early if the next token is not present. If it is present, the rest of the instructions will execute.
             */
            range_parser &if_next_token_present(const cstring &token);

            /**
             * Skip the next tokens by count. These tokens will not be captured (aka ignored).
             */
            range_parser &skip_tokens_by(size_t count);

            /**
             * Skip the next tokens until the given token is found. These tokens will not be captured (aka ignored).
             */
            range_parser &skip_token_until(const cstring &token);

            /**
             * Take the next tokens until the given token is found. These tokens will be captured under the provided key.
             */
            range_parser &take_token_until(const cstring &token, const cstring &key);

            /**
             * Optionally capture the range, if it is present. Otherwise proceed to the next token.
             */
            range_parser &optional_capture_range(const cstring &from, const cstring &to, const cstring &key);

            /**
             * Capture the range. These tokens will be captured under the provided key. The try_capture method will return early if it fails and it's result will have the matches field set to false.
             */
            range_parser &capture_range(const cstring &from, const cstring &to, const cstring &key);

            /**
             * Optionally take the next tokens until the given token is found IF the previous range key is present. These tokens will be captured under the provided key.
             */
            range_parser &optional_take_until_if_previous_key(const cstring &previous_key, const cstring &until_token, const cstring &key);

            /**
             * This allows to set a type identifier for the captured/parsed range.
             */
            range_parser &set_type(const cstring &type_key);

            /**
             * Executes the parser using the provided rules.
             */
            [[nodiscard]] parsed_ranges try_capture(const std::vector<language_token> &tokens, size_t from_index, size_t to_index) const;
    };
}

#endif //SHADE_RANGE_CAPTURE_H
