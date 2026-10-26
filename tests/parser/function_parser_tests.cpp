//
// Created by Laurens Ruijtenberg on 25/10/2026.
//

#include <gtest/gtest.h>

#include <shared/utils/cstring.hpp>
#include <compiler/lexer/lexer.hpp>
#include <compiler/parser/parser.hpp>
#include <compiler/parser/parser_range_keys.hpp>
#include <compiler/parser/functionparser.hpp>
#include <compiler/meta/function_definition.hpp>
#include <compiler/ast/ast_node.hpp>

TEST(function_parser, basic_parser) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func a() {
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens.size(), 6);

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();
    parse.analyze_main_areas();

    auto & program = context.get_program_structure();
    auto function_def = program.get_function_definition("a");

    ASSERT_NE(function_def, nullptr);
    ASSERT_EQ(function_def->name, "a");
    ASSERT_EQ(function_def->arguments.size(), 0);
    ASSERT_EQ(function_def->return_type.is_void(), true);
}

TEST(function_parser, basic_return) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func a() -> bool {
            return true;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();
    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();
    parse.analyze_main_areas();

    auto & program = context.get_program_structure();
    auto function_def = program.get_function_definition("a");
    bool fp_result = shade::function_parser::parse(tokens, parse_map.front(), context, *function_def);

    ASSERT_TRUE(fp_result);
    ASSERT_NE(function_def, nullptr);
    ASSERT_EQ(function_def->name, "a");
    ASSERT_EQ(function_def->arguments.size(), 0);
    ASSERT_EQ(function_def->return_type.is_void(), false);
    ASSERT_EQ(function_def->return_type.is_ref, false);
    ASSERT_EQ(function_def->return_type.is_array, false);
    ASSERT_EQ(function_def->return_type.is_const, false);
    ASSERT_EQ(function_def->return_type.is_dynamic_array, false);
    ASSERT_EQ(function_def->return_type.is_owned, false);
    ASSERT_EQ(function_def->return_type.is_ptr, false);
    ASSERT_EQ(function_def->return_type.fixed_array_size, 0);
    ASSERT_EQ(function_def->has_return_type, true);
    ASSERT_NE(function_def->return_type.type, nullptr);
    ASSERT_EQ(function_def->return_type.type->is_primitive, true);
    ASSERT_EQ(function_def->return_type.type->primitive_type, shade::primitive_types::boolean);
}

TEST(function_parser, basic_params) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func a(i32 b, i32 c) -> bool {
            return true;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens.size(), 16);

    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();
    parse.analyze_main_areas();

    auto & program = context.get_program_structure();
    auto function_def = program.get_function_definition("a");
    bool fp_result = shade::function_parser::parse(tokens, parse_map.front(), context, *function_def);

    ASSERT_TRUE(fp_result);

    ASSERT_EQ(function_def->arguments.size(), 2);
    ASSERT_EQ(function_def->arguments[0].name, "b");
    ASSERT_EQ(function_def->arguments[1].name, "c");
    ASSERT_EQ(function_def->arguments[0].type.type->primitive_type, shade::primitive_types::i32);
    ASSERT_EQ(function_def->arguments[1].type.type->primitive_type, shade::primitive_types::i32);
    ASSERT_EQ(function_def->has_return_type, true);
    ASSERT_EQ(function_def->return_type.type->primitive_type, shade::primitive_types::boolean);
}

TEST(function_parser, basic_param_fault) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func a(,) -> bool {
            return true;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();
    auto parse = shade::parser(context, tokens);
    auto parse_map = parse.capture_main_areas();
    parse.analyze_main_areas();

    auto & program = context.get_program_structure();
    auto function_def = program.get_function_definition("a");
    bool fp_result = shade::function_parser::parse(tokens, parse_map.front(), context, *function_def);

    ASSERT_FALSE(fp_result);
}