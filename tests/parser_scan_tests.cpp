//
// Created by Laurens Ruijtenberg on 13/09/2026.
//

#include <gtest/gtest.h>

#include <lexer/lexer.hpp>
#include <parser/parser.hpp>

#include "parser/parser_range_keys.hpp"

TEST(parser_analysis, top_level_first_pass) {
    shade::compile_context context;

    constexpr const char * source = R"(
        struct a {
        }

        struct b {
        }

        struct c {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.first_pass();

    parse.analyze_first_pass();

    auto & types = context.get_type_index();
    auto a_type = types.get_typeinformation("a");
    auto b_type = types.get_typeinformation("b");
    auto c_type = types.get_typeinformation("c");

    ASSERT_NE(a_type, nullptr);
    ASSERT_NE(b_type, nullptr);
    ASSERT_NE(c_type, nullptr);
}

TEST(parser_analysis, inside_of_module) {
    shade::compile_context context;

    constexpr const char * source = R"(
        module m {
            struct a {
            }

            struct b {
            }

            struct c {
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.first_pass();

    parse.analyze_first_pass();

    auto & types = context.get_type_index();
    auto a_type = types.get_typeinformation("m::a");
    auto b_type = types.get_typeinformation("m::b");
    auto c_type = types.get_typeinformation("m::c");

    ASSERT_NE(a_type, nullptr);
    ASSERT_NE(b_type, nullptr);
    ASSERT_NE(c_type, nullptr);
}

TEST(parser_analysis, inside_of_module_mixed) {
    shade::compile_context context;

    constexpr const char * source = R"(
        module m {

            module n {
                struct a {
                }
            }

            struct b {
            }

            struct c {
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.first_pass();

    parse.analyze_first_pass();

    auto & types = context.get_type_index();
    auto a_type = types.get_typeinformation("m::n::a");
    auto b_type = types.get_typeinformation("m::b");
    auto c_type = types.get_typeinformation("m::c");

    ASSERT_NE(a_type, nullptr);
    ASSERT_NE(b_type, nullptr);
    ASSERT_NE(c_type, nullptr);
}

TEST(parser_analysis, functions) {
    shade::compile_context context;

    constexpr const char * source = R"(
        module m {

            module n {
                struct a {
                }
            }

            struct b {
            }

            struct c {
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.first_pass();

    parse.analyze_first_pass();

    auto & types = context.get_type_index();
    auto a_type = types.get_typeinformation("m::n::a");
    auto b_type = types.get_typeinformation("m::b");
    auto c_type = types.get_typeinformation("m::c");

    ASSERT_NE(a_type, nullptr);
    ASSERT_NE(b_type, nullptr);
    ASSERT_NE(c_type, nullptr);
}