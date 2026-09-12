//
// Created by Laurens Ruijtenberg on 12/09/2026.
//
#include <gtest/gtest.h>

#include <lexer/lexer.hpp>
#include <parser/parser.hpp>

#include "parser/parser_range_keys.hpp"

TEST(parser, basic_module) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context);
    auto parse_map = parse.first_pass(tokens, 0, tokens.size() -1);

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());
}


TEST(parser, nested_module_2) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name {
            module subname {
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context);
    auto parse_map = parse.first_pass(tokens, 0, tokens.size() -1);

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());
}

TEST(parser, basic_struct) {
    shade::compile_context context;
    constexpr const char * source = R"(
        struct name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context);
    auto parse_map = parse.first_pass(tokens, 0, tokens.size() -1);

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::struct_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::struct_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::struct_body).has_value());
}