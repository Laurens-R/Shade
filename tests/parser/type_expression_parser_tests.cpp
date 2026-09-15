//
// Created by Laurens Ruijtenberg on 15/09/2026.
//
#include <gtest/gtest.h>

#include <lexer/lexer.hpp>
#include <parser/typeexpressionparser.hpp>

TEST(type_expression_parser, basic_expression) {
    shade::compile_context context;
    constexpr const char * source = R"(
        i32 a
    )";

    shade::compile_context contex;

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    auto detected_type = shade::type_expression_parser::parse(tokens, 0, tokens.size() - 1, context, shade::cstring::empty());

    ASSERT_EQ(detected_type.is_array, false);
    ASSERT_EQ(detected_type.fixed_array_size, false);
    ASSERT_EQ(detected_type.is_const, false);
    ASSERT_EQ(detected_type.is_dynamic_array, false);
    ASSERT_EQ(detected_type.is_owning, false);
    ASSERT_EQ(detected_type.is_ptr, false);
    //ASSERT_NE(detected_type.type, nullptr);

}
