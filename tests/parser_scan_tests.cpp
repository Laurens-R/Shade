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

    auto & ps = context.get_program_structure();
    auto a_type = ps.get_type_definition("a");
    auto b_type = ps.get_type_definition("b");
    auto c_type = ps.get_type_definition("c");

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

    auto & ps = context.get_program_structure();
    auto a_type = ps.get_type_definition("m::a");
    auto b_type = ps.get_type_definition("m::b");
    auto c_type = ps.get_type_definition("m::c");

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

    auto & ps = context.get_program_structure();
    auto a_type = ps.get_type_definition("m::n::a");
    auto b_type = ps.get_type_definition("m::b");
    auto c_type = ps.get_type_definition("m::c");

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
                    func f1<templated>() {
                    }
                }
            }

            struct b {
            }

            struct c {
                func f2 () -> i32 {
                }
            }

            func f3() {
            }
        }

        func f4() {
        }

        struct d {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.first_pass();

    parse.analyze_first_pass();

    auto & ps = context.get_program_structure();
    auto a_type = ps.get_type_definition("m::n::a");
    auto b_type = ps.get_type_definition("m::b");
    auto c_type = ps.get_type_definition("m::c");
    auto d_type = ps.get_type_definition("d");

    ASSERT_NE(a_type, nullptr);
    ASSERT_NE(b_type, nullptr);
    ASSERT_NE(c_type, nullptr);
    ASSERT_NE(d_type, nullptr);

    auto f1 = ps.get_function_definition("m::n::a::f1");
    auto f2 = ps.get_function_definition("m::c::f2");
    auto f3 = ps.get_function_definition("m::f3");
    auto f4 = ps.get_function_definition("f4");

    ASSERT_NE(f1, nullptr);
    ASSERT_NE(f2, nullptr);
    ASSERT_NE(f3, nullptr);
    ASSERT_NE(f4, nullptr);
}