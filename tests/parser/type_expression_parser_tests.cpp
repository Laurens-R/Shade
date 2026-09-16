//
// Created by Laurens Ruijtenberg on 15/09/2026.
//
#include <gtest/gtest.h>

#include <compiler/lexer/lexer.hpp>
#include <compiler/parser/typeexpressionparser.hpp>

TEST(type_expression_parser, basic_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, false);
    ASSERT_NE(detected_type->type, nullptr);

}

TEST(type_expression_parser, const_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        const i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, true);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, false);
    ASSERT_NE(detected_type->type, nullptr);

}

TEST(type_expression_parser, ref_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        ref i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, true);
    ASSERT_NE(detected_type->type, nullptr);

}

TEST(type_expression_parser, ptr_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        ptr i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, true);
    ASSERT_EQ(detected_type->is_ref, false);

    //ASSERT_NE(detected_type.type, nullptr);
}

TEST(type_expression_parser, const_ptr_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        const ptr i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, true);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, true);
    ASSERT_EQ(detected_type->is_ref, false);
    ASSERT_NE(detected_type->type, nullptr);
}

TEST(type_expression_parser, const_ref_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        const ref i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, true);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, true);
}

TEST(type_expression_parser, owning_ref_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        owned ref i32
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, true);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, true);
}

TEST(type_expression_parser, basic_static_array) {
    shade::compile_context context;
    constexpr const char * source = R"(
        i32[5]
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, true);
    ASSERT_EQ(detected_type->fixed_array_size, 5);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, false);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, false);
}

TEST(type_expression_parser, basic_dynamic_array) {
    shade::compile_context context;
    constexpr const char * source = R"(
        i32[]
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type->is_array, false);
    ASSERT_EQ(detected_type->fixed_array_size, 0);
    ASSERT_EQ(detected_type->is_const, false);
    ASSERT_EQ(detected_type->is_dynamic_array, true);
    ASSERT_EQ(detected_type->is_owned, false);
    ASSERT_EQ(detected_type->is_ptr, false);
    ASSERT_EQ(detected_type->is_ref, false);

}


