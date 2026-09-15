//
// Created by laure on 10/09/2026.
//

#include "range_parser.hpp"

#include <ranges>

namespace shade {

    void parsed_ranges::reset() {
        _captured_ranges.clear();
        _captured_ranges_by_key.clear();
    }

    void parsed_ranges::add_captured_range(size_t from, size_t to, const cstring &key) {
        _captured_ranges.emplace_back(key, from, to);
        _captured_ranges_by_key.insert_or_assign(key.to_c_string(), _captured_ranges.size() - 1);
    }

    std::optional<parsed_range> parsed_ranges::get_captured_range(const cstring &key) {
        if (!_captured_ranges_by_key.contains(key.to_c_string())) {
            return std::nullopt;
        }
        return _captured_ranges[_captured_ranges_by_key.at(key.to_c_string())];
    }

    bool parsed_ranges::contains_key(const cstring &key) {
        return _captured_ranges_by_key.contains(key.to_c_string());
    }

    size_t parsed_ranges::get_size() {
        return _captured_ranges.size();
    }

    std::optional<parsed_range> parsed_ranges::get_captured_range_at(const size_t index) {
        if (index >= _captured_ranges.size()) {
            return std::nullopt;
        }
        return _captured_ranges[index];
    }

    range_parser &range_parser::optional_match_and_capture_token(const cstring &token, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::optional_match_single,
            .match_tokens = {token},
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::match_and_capture_token(const cstring &token, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::match_single,
            .match_tokens = {token},
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::match_and_capture_any_token(const std::vector<cstring> &tokens, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::match_any,
            .match_tokens = tokens,
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::capture_tokens_by_count(size_t count, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::take_n,
            .match_tokens = {},
            .skip_count = count,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::if_next_token_present(const cstring &token) {
        _parse_instructions.push_back({
            .type = instruction_type::if_next_token_present,
            .match_tokens = {token},
            .skip_count = 0,
            .key = cstring::empty(),
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::skip_tokens_by(size_t count) {
        _parse_instructions.push_back({
            .type = instruction_type::skip_count,
            .match_tokens = {},
            .skip_count = count,
            .key = cstring::empty(),
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::skip_token_until(const cstring &token) {
        _parse_instructions.push_back({
            .type = instruction_type::skip_until,
            .match_tokens = {token},
            .skip_count = 0,
            .key = cstring::empty(),
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::take_token_until(const cstring &token, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::take_until,
            .match_tokens = {token},
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::optional_capture_range(const cstring &from, const cstring &to, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::optional_match_range,
            .match_tokens = {from, to},
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::capture_range(const cstring &from, const cstring &to, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::match_range,
            .match_tokens = {from, to},
            .skip_count = 0,
            .key = key,
            .previous_key = cstring::empty()
        });
        return *this;
    }

    range_parser &range_parser::optional_take_until_if_previous_key(const cstring &previous_key, const cstring &until_token, const cstring &key) {
        _parse_instructions.push_back({
            .type = instruction_type::optional_take_until_if_previous_key,
            .match_tokens = {until_token},
            .skip_count = 0,
            .key = key,
            .previous_key = previous_key
        });

        return *this;
    }

    range_parser &range_parser::set_type(const cstring &type_key) {
        capture_type_key = type_key;
        return *this;
    }

    parsed_ranges range_parser::try_capture(const std::vector<language_token> &tokens, size_t from_index, size_t to_index) const {
        parsed_ranges results;
        results.type_key = capture_type_key;

        if (from_index > to_index) {
            results.failure_message = "Invalid range: from_index must be less than to_index";
            results.matched         = false;
            return results;
        }

        if (from_index >= tokens.size() || to_index > tokens.size()) {
            results.failure_message = "Invalid range: from_index or to_index is out of bounds";
            results.matched         = false;
            return results;
        }

        results.range_from_index   = from_index;
        size_t current_token_index = from_index;

        auto contains_token = [](const cstring &token, const std::vector<cstring> &tokens_to_check) -> bool {
            return std::find(tokens_to_check.begin(), tokens_to_check.end(), token) != tokens_to_check.end();
        };

        for (auto &instruction: _parse_instructions) {
            bool matched = false;

            if (current_token_index > to_index) {
                results.failure_message = "Range not found within tokens.";
                matched                 = false;
                break;
            }

            bool stop_iterating = false;

            switch (instruction.type) {
                case instruction_type::optional_match_single: {
                    if (contains_token(tokens[current_token_index].text, instruction.match_tokens)) {
                        results.add_captured_range(current_token_index, current_token_index, instruction.key);
                        matched = true;
                    } else {
                        continue;
                    }
                }
                case instruction_type::match_single: {
                    if (contains_token(tokens[current_token_index].text, instruction.match_tokens)) {
                        results.add_captured_range(current_token_index, current_token_index, instruction.key);
                        matched = true;
                    } else {
                        results.failure_message = "Failed to match single token.";
                        matched                 = false;
                        break;
                    }
                    current_token_index++;

                }
                break;
                case instruction_type::match_any: {
                    if (contains_token(tokens[current_token_index].text, instruction.match_tokens)) {
                        results.add_captured_range(current_token_index, current_token_index, instruction.key);
                        matched = true;
                    } else {
                        results.failure_message = "Failed to match any of the specified tokens.";
                        matched                 = false;
                        break;
                    }
                    current_token_index++;

                }
                break;
                case instruction_type::optional_match_range: {
                    if (!(instruction.match_tokens.size() == 2)) {
                        results.failure_message = "Invalid range capture instruction.";
                        matched                 = false;
                        break;
                    }

                    auto from_token = instruction.match_tokens.at(0);
                    auto to_token   = instruction.match_tokens.at(1);

                    if (tokens[current_token_index].text != from_token) {
                        continue;
                    }
                }
                //we faill through on purpose so we don't have to duplicate
                //the range logic.
                case instruction_type::match_range: {
                    if (!(instruction.match_tokens.size() == 2)) {
                        results.failure_message = "Invalid range capture instruction.";
                        matched                 = false;
                        break;
                    }

                    auto from_token = instruction.match_tokens.at(0);
                    auto to_token   = instruction.match_tokens.at(1);

                    if (tokens[current_token_index].text != from_token) {
                        results.failure_message = "Did not find expected opening token at start of range.";
                        matched                 = false;
                        break;
                    }

                    auto open_count        = 1;
                    auto close_count       = 0;
                    auto start_token_index = current_token_index;

                    //increase the current token index by one, because we already matched the first token
                    current_token_index++;

                    while (open_count > close_count && current_token_index <= to_index) {
                        if (tokens[current_token_index].text == to_token) {
                            close_count++;
                        } else if (tokens[current_token_index].text == from_token) {
                            open_count++;
                        }

                        if (open_count != close_count) {
                            current_token_index++;
                        }
                    }

                    results.add_captured_range(start_token_index, current_token_index, instruction.key);
                    current_token_index++;
                    matched = true;
                }
                break;
                case instruction_type::take_n: {
                    //we are doing a -1 correction because we are including the token at the current index
                    if ((current_token_index + instruction.skip_count - 1) > to_index) {
                        results.failure_message = "Skipped beyond token range.";
                        matched                 = false;
                        break;
                    }

                    size_t start_index = current_token_index;
                    results.add_captured_range(start_index, start_index + instruction.skip_count - 1, instruction.key);

                    current_token_index += instruction.skip_count;


                    matched = true;
                }
                break;
                case instruction_type::if_next_token_present: {
                    if (contains_token(tokens[current_token_index].text, instruction.match_tokens)) {

                        if (current_token_index > to_index) {
                            results.failure_message = "Range not found within tokens.";
                            matched                 = false;
                            break;
                        }

                        matched = true;
                    } else {
                        stop_iterating = true;
                        matched        = true;
                        break;
                    }
                }
                break;
                case instruction_type::skip_count: {
                    //we are doing a -1 correction because we are including the token at the current index
                    if ((current_token_index + instruction.skip_count - 1) > to_index) {
                        results.failure_message = "Skipped beyond token range.";
                        matched                 = false;
                        break;
                    }
                    current_token_index += instruction.skip_count;
                    matched             = true;
                }
                break;
                case instruction_type::optional_take_until_if_previous_key: {
                    auto result_size = results.get_size();
                    if (result_size == 0) {
                        continue;
                    }

                    auto previous = results.get_captured_range_at(result_size - 1);
                    if (!previous) {
                        continue;
                    }

                    auto previous_value = previous.value();

                    if (previous_value.key != instruction.previous_key) {
                        continue;
                    }
                }
                //we purposefully fall through here to leverage the existing skip unit logic
                case instruction_type::take_until:
                case instruction_type::skip_until: {
                    if (instruction.match_tokens.empty()) {
                        results.failure_message = "No tokens to skip until.";
                        matched                 = false;
                        break;
                    }

                    auto &token_to_find = instruction.match_tokens.front();

                    auto start_token_index = current_token_index;

                    bool found = false;
                    while (current_token_index <= to_index) {

                        if (token_to_find == tokens[current_token_index].text) {
                            found = true;
                            break;
                        }

                        current_token_index++;
                    }

                    if (!found) {
                        results.failure_message = "Range not found within tokens.";
                        matched                 = false;
                        break;
                    } else {
                        matched = true;

                        if (instruction.type == instruction_type::optional_take_until_if_previous_key
                            || instruction.type == instruction_type::take_until) {
                            results.add_captured_range(start_token_index, current_token_index, instruction.key);
                        }
                    }
                }
                break;
                case instruction_type::undefined:
                default:
                    break;
            }

            if (!matched) {
                results.reset();
                results.matched = false;
                break;
            }

            if (stop_iterating) {
                break;
            }
        }

        results.range_to_index = current_token_index;

        return results;
    }


}
