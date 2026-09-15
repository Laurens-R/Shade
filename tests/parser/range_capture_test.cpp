//
// Created by Laurens Ruijtenberg on 12/09/2026.
//


#include <gtest/gtest.h>

#include <lexer/lexer.hpp>
#include <parser/range_parser.hpp>
#include <lang/spelling.hpp>

TEST(range_capture, basic_match) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a b c
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.match_and_capture_token("a", "a")
    .match_and_capture_token("b", "b")
    .match_and_capture_token("c", "c")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("a").has_value());
    ASSERT_TRUE(result.get_captured_range("b").has_value());
    ASSERT_TRUE(result.get_captured_range("c").has_value());

    auto a_range = result.get_captured_range("a").value();
    auto b_range = result.get_captured_range("b").value();
    auto c_range = result.get_captured_range("c").value();

    ASSERT_EQ(a_range.from_token_index, 0);
    ASSERT_EQ(a_range.to_token_index, 0);
    ASSERT_EQ(b_range.from_token_index, 1);
    ASSERT_EQ(b_range.to_token_index, 1);
    ASSERT_EQ(c_range.from_token_index, 2);
    ASSERT_EQ(c_range.to_token_index, 2);

    ASSERT_EQ(tokens.at(a_range.from_token_index).text, "a");
    ASSERT_EQ(tokens.at(b_range.from_token_index).text, "b");
    ASSERT_EQ(tokens.at(c_range.from_token_index).text, "c");
}

TEST(range_capture, basic_match_any) {
    shade::compile_context context;
    constexpr const char * source = R"(
        b
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.match_and_capture_any_token({ "a", "b", "c"}, "b")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("b").has_value());

    auto a_range = result.get_captured_range("b").value();

    ASSERT_EQ(a_range.from_token_index, 0);
    ASSERT_EQ(a_range.to_token_index, 0);

    ASSERT_EQ(tokens.at(a_range.from_token_index).text, "b");

}

TEST(range_capture, capture_tokens_by_count) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a b c d
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.capture_tokens_by_count(4, "bycount")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("bycount").has_value());

    auto a_range = result.get_captured_range("bycount").value();

    ASSERT_EQ(a_range.from_token_index, 0);
    ASSERT_EQ(a_range.to_token_index, 3);

    ASSERT_EQ(tokens.at(0).text, "a");
    ASSERT_EQ(tokens.at(1).text, "b");
    ASSERT_EQ(tokens.at(2).text, "c");
    ASSERT_EQ(tokens.at(3).text, "d");
}

TEST(range_capture, skip_by_count) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a b c d
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.skip_tokens_by(3)
    .match_and_capture_token("d", "d")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("d").has_value());

    auto d_range = result.get_captured_range("d").value();

    ASSERT_EQ(d_range.from_token_index, 3);
    ASSERT_EQ(d_range.to_token_index, 3);
}

TEST(range_capture, skip_until) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a b c d
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.skip_token_until("d")
    .match_and_capture_token("d", "d")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("d").has_value());

    auto d_range = result.get_captured_range("d").value();

    ASSERT_EQ(d_range.from_token_index, 3);
    ASSERT_EQ(d_range.to_token_index, 3);
}

TEST(range_capture, capture_range) {
    shade::compile_context context;
    constexpr const char * source = R"(
        {
            a
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.capture_range("{", "}", "r")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("r").has_value());

    auto range = result.get_captured_range("r").value();

    ASSERT_EQ(range.from_token_index, 0);
    ASSERT_EQ(range.to_token_index, 2);
}

TEST(range_capture, capture_range_nested) {
    shade::compile_context context;
    constexpr const char * source = R"(
        {
            {
                a
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.capture_range("{", "}", "r")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("r").has_value());

    auto range = result.get_captured_range("r").value();

    ASSERT_EQ(range.from_token_index, 0);
    ASSERT_EQ(range.to_token_index, 4);
}

TEST(range_capture, if_next_token_present) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a b c
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.match_and_capture_token("a", "a")
    .if_next_token_present("b")
    .match_and_capture_token("b", "b")
    .match_and_capture_token("c", "c")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("a").has_value());
    ASSERT_TRUE(result.get_captured_range("b").has_value());
    ASSERT_TRUE(result.get_captured_range("c").has_value());
}

TEST(range_capture, if_next_token_not_present) {
    shade::compile_context context;
    constexpr const char * source = R"(
        a c
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    auto result = capture.match_and_capture_token("a", "a")
    .if_next_token_present("b")
    .match_and_capture_token("b", "b")
    .match_and_capture_token("c", "c")
    .try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("a").has_value());
    ASSERT_TRUE(!result.get_captured_range("b").has_value());
    ASSERT_TRUE(!result.get_captured_range("c").has_value());
}

TEST(range_capture, module_test) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    shade::range_parser capture;
    capture.match_and_capture_token(shade::spelling::modules::module_name, "mkeyword")
    .capture_tokens_by_count(1, "mname")
    .capture_range(shade::spelling::modules::begin_body, shade::spelling::modules::end_body, "mbody");
    auto result = capture.try_capture(tokens, 0, tokens.size() - 1);

    ASSERT_EQ(result.matched, true);
    ASSERT_EQ(result.failure_message, shade::cstring::empty());
    ASSERT_TRUE(result.get_captured_range("mkeyword").has_value());
    ASSERT_TRUE(result.get_captured_range("mname").has_value());
    ASSERT_TRUE(result.get_captured_range("mbody").has_value());
}

