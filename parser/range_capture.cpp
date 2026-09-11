//
// Created by laure on 10/09/2026.
//

#include "range_capture.h"

namespace shade {

    range_capture& range_capture::if_start_with(const cstring & token) {
        _starts_with = token;
        return *this;
    }

    range_capture& range_capture::then_capture_range(const cstring & from, const cstring & to, const range_type type) {
        range_tokens t = {
            .from = from, .to = to, .type = type
        };

        _ranges.emplace_back(t);
        return *this;
    }

    std::optional<captured_range> range_capture::try_capture(const std::vector<language_token>& tokens, const size_t start_offset, const size_t stop_index) const {
        auto& token = tokens[start_offset];

        if (token.text == _starts_with) {
            captured_range result;
            result.top_level_prelude = token.text;
            result.from_token_index = start_offset;

            size_t current_token_index = start_offset + 1;

            if (_ranges.empty()) {
                return std::nullopt;
            }

            auto& first_range = _ranges.front();

            while (tokens[current_token_index].text != first_range.from && current_token_index < stop_index) {
                current_token_index++;
            }

            if (current_token_index == stop_index) {
                return std::nullopt;
            }

            for (auto& range : _ranges) {
                size_t from_counter = 0;
                size_t to_counter   = 0;

                auto& expected_next_token = range.from;
                auto& actual_next_token   = tokens[current_token_index].text;

                if (actual_next_token == expected_next_token) {
                    token_range result_range;
                    result_range.from_token_index = current_token_index;
                    result_range.type             = range.type;

                    from_counter++;

                    while (from_counter != to_counter) {
                        if ((tokens.size() - 1) > current_token_index) {
                            current_token_index++;
                            if (tokens[current_token_index].text == range.from) {
                                from_counter++;
                            } else if (tokens[current_token_index].text == range.to) {
                                to_counter++;
                            }
                        } else {
                            break;
                        }
                    }

                    result_range.to_token_index = current_token_index;
                    result.ranges.emplace_back(result_range);
                }
            }

            result.to_token_index = current_token_index;

            return result;
        } else {
            return std::nullopt;
        }
    }
}
