//
// Created by Laurens Ruijtenberg on 12/09/2026.
//
#include <gtest/gtest.h>

#include <lexer/lexer.hpp>
#include <parser/parser.hpp>

#include "parser/parser_range_keys.hpp"

TEST(parser, basic_variable) {
    shade::compile_context context;
    constexpr const char * source = R"(
        i32 test;
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & var_result = parse_map.at(0);
    ASSERT_EQ(var_result.type_key, shade::range_keys::statement_type_key);

    ASSERT_EQ(var_result.range_from_index, 0);
    ASSERT_EQ(var_result.range_to_index, 2);
}

TEST(parser, basic_if) {
    shade::compile_context context;
    constexpr const char * source = R"(
        if(a) {
        };
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & if_result = parse_map.at(0);
    ASSERT_EQ(if_result.type_key, shade::range_keys::if_type_key);

    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_condition).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_body).has_value());
}

TEST(parser, if_else) {
    shade::compile_context context;
    constexpr const char * source = R"(
        if(a) {
        } else {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 2);

    auto & if_result = parse_map.at(0);
    ASSERT_EQ(if_result.type_key, shade::range_keys::if_type_key);

    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_condition).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_body).has_value());

    auto & else_result = parse_map.at(1);
    ASSERT_EQ(else_result.type_key, shade::range_keys::else_type_key);

    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::else_keyword).has_value());
    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::else_body).has_value());
}

TEST(parser, if_else_if) {
    shade::compile_context context;
    constexpr const char * source = R"(
        if(a) {
        } else if (b) {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 2);

    auto & if_result = parse_map.at(0);
    ASSERT_EQ(if_result.type_key, shade::range_keys::if_type_key);

    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_condition).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_body).has_value());

    auto & else_result = parse_map.at(1);
    ASSERT_EQ(else_result.type_key, shade::range_keys::if_else_type_key);

    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::else_keyword).has_value());
    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::if_else_condition).has_value());
    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::if_else_body).has_value());

}

TEST(parser, if_else_if_else) {
    shade::compile_context context;
    constexpr const char * source = R"(
        if(a) {
        } else if (b) {
        } else {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 3);

    auto & if_result = parse_map.at(0);
    ASSERT_EQ(if_result.type_key, shade::range_keys::if_type_key);

    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_condition).has_value());
    ASSERT_TRUE(if_result.get_captured_range(shade::range_keys::if_body).has_value());

    auto & else_if_result = parse_map.at(1);
    ASSERT_EQ(else_if_result.type_key, shade::range_keys::if_else_type_key);

    ASSERT_TRUE(else_if_result.get_captured_range(shade::range_keys::else_keyword).has_value());
    ASSERT_TRUE(else_if_result.get_captured_range(shade::range_keys::if_keyword).has_value());
    ASSERT_TRUE(else_if_result.get_captured_range(shade::range_keys::if_else_condition).has_value());
    ASSERT_TRUE(else_if_result.get_captured_range(shade::range_keys::if_else_body).has_value());

    auto & else_result = parse_map.at(2);
    ASSERT_EQ(else_result.type_key, shade::range_keys::else_type_key);

    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::else_keyword).has_value());
    ASSERT_TRUE(else_result.get_captured_range(shade::range_keys::else_body).has_value());
}

TEST(parser, basic_for) {
    shade::compile_context context;
    constexpr const char * source = R"(
        for(i32 i = 0; i < 10; ++i) {
        };
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & for_result = parse_map.at(0);
    ASSERT_EQ(for_result.type_key, shade::range_keys::for_type_key);

    ASSERT_TRUE(for_result.get_captured_range(shade::range_keys::for_loop_keyword).has_value());
    ASSERT_TRUE(for_result.get_captured_range(shade::range_keys::for_loop_instructions).has_value());
    ASSERT_TRUE(for_result.get_captured_range(shade::range_keys::for_loop_body).has_value());
}

TEST(parser, basic_while) {
    shade::compile_context context;
    constexpr const char * source = R"(
        while(some_condition) {
        };
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & while_result = parse_map.at(0);
    ASSERT_EQ(while_result.type_key, shade::range_keys::while_type_key);

    ASSERT_TRUE(while_result.get_captured_range(shade::range_keys::while_loop_keyword).has_value());
    ASSERT_TRUE(while_result.get_captured_range(shade::range_keys::while_loop_condition).has_value());
    ASSERT_TRUE(while_result.get_captured_range(shade::range_keys::while_loop_body).has_value());
}

TEST(parser, basic_module) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

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

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());

    ASSERT_EQ(module_result.child_results.size(), 1);

    auto & child_result = module_result.child_results.at(0);

    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::module_body).has_value());

    auto submodule_name_result = child_result.get_captured_range(shade::range_keys::module_name).value();

    ASSERT_EQ(tokens[submodule_name_result.from_token_index].text, "subname");
}

TEST(parser, multiple_modules) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module a { }

        module b { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 2);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());

    auto & module2_result = parse_map.at(1);

    ASSERT_EQ(module2_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module2_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module2_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module2_result.get_captured_range(shade::range_keys::module_body).has_value());
}

TEST(parser, module_with_variable) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name {
            i32 test;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());

    ASSERT_EQ(module_result.child_results.size(), 1);

    auto & child_result = module_result.child_results.at(0);

}

TEST(parser, basic_struct) {
    shade::compile_context context;
    constexpr const char * source = R"(
        struct name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & struct_result = parse_map.at(0);

    ASSERT_EQ(struct_result.type_key, shade::range_keys::struct_type_key);

    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_keyword).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_name).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_body).has_value());
}

TEST(parser, bare_function) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func test(i32 a, i32 b) {
            i32 c = a + b;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & func_result = parse_map.at(0);

    ASSERT_EQ(func_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_arguments).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_body).has_value());

    ASSERT_EQ(func_result.child_results.size(), 1);
}

TEST(parser, bare_function_with_return_type) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func test(i32 a, i32 b) -> i32 {
            i32 c = a + b;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & func_result = parse_map.at(0);

    ASSERT_EQ(func_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_arguments).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_return_type_indicator).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_return_type).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_body).has_value());

    ASSERT_EQ(func_result.child_results.size(), 1);
}

TEST(parser, bare_function_with_generics_and_return_type) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func test<Ta, Tb>(i32 a, i32 b) -> i32 {
            i32 c = a + b;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & func_result = parse_map.at(0);

    ASSERT_EQ(func_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_generic_arguments).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_arguments).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_return_type_indicator).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_return_type).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_body).has_value());

    ASSERT_EQ(func_result.child_results.size(), 1);
}

TEST(parser, struct_with_func) {
    shade::compile_context context;
    constexpr const char * source = R"(
        struct name {
            func test(i32 a, i32 b) {
                i32 c = a + b;
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & struct_result = parse_map.at(0);

    ASSERT_EQ(struct_result.type_key, shade::range_keys::struct_type_key);

    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_keyword).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_name).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_body).has_value());

    ASSERT_EQ(struct_result.child_results.size(), 1);

    auto & child_result = struct_result.child_results.at(0);

    ASSERT_EQ(child_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_keyword).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_body).has_value());

    auto function_result = child_result.get_captured_range(shade::range_keys::function_name).value();

    ASSERT_EQ(tokens[function_result.from_token_index].text, "test");

}

TEST(parser, module_with_func) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name {
            func test(i32 a, i32 b) {
                i32 c = a + b;
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());

    ASSERT_EQ(module_result.child_results.size(), 1);

    auto & child_result = module_result.child_results.at(0);

    ASSERT_EQ(child_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_keyword).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(child_result.get_captured_range(shade::range_keys::function_body).has_value());

    auto function_result = child_result.get_captured_range(shade::range_keys::function_name).value();

    ASSERT_EQ(tokens[function_result.from_token_index].text, "test");

}

TEST(parser, module_struct_func) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module m {
            struct s {
                func f(i32 a, i32 b) {
                    i32 c = a + b;
                }
            }
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();

    ASSERT_TRUE(parse_map.size() == 1);

    auto & module_result = parse_map.at(0);

    ASSERT_EQ(module_result.type_key, shade::range_keys::module_type_key);

    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_keyword).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_name).has_value());
    ASSERT_TRUE(module_result.get_captured_range(shade::range_keys::module_body).has_value());

    ASSERT_EQ(module_result.child_results.size(), 1);

    auto & struct_result = module_result.child_results.at(0);

    ASSERT_EQ(struct_result.type_key, shade::range_keys::struct_type_key);

    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_keyword).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_name).has_value());
    ASSERT_TRUE(struct_result.get_captured_range(shade::range_keys::struct_body).has_value());

    ASSERT_EQ(struct_result.child_results.size(), 1);

    auto & func_result = struct_result.child_results.at(0);
    ASSERT_EQ(func_result.type_key, shade::range_keys::function_type_key);

    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_keyword).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_name).has_value());
    ASSERT_TRUE(func_result.get_captured_range(shade::range_keys::function_body).has_value());

    auto function_result = func_result.get_captured_range(shade::range_keys::function_name).value();

    ASSERT_EQ(tokens[function_result.from_token_index].text, "f");
}

