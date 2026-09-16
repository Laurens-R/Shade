//
// Created by Laurens Ruijtenberg on 12/09/2026.
//
#include <gtest/gtest.h>

#include <compiler/lexer/lexer.hpp>

TEST(lexer, basic_tokenization) {
    shade::compile_context context;
    constexpr const char * source = R"(
        module name { }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens.size(), 4);
    ASSERT_EQ(tokens[0].text, "module");
    ASSERT_EQ(tokens[1].text, "name");
    ASSERT_EQ(tokens[2].text, "{");
    ASSERT_EQ(tokens[3].text, "}");
}

TEST(lexer, basic_tokenization_with_comments) {
    shade::compile_context context;
    constexpr const char * source = R"(
        //test comment
        module name { }
        /* multiline
           comment */
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens.size(), 4);
    ASSERT_EQ(tokens[0].text, "module");
    ASSERT_EQ(tokens[1].text, "name");
    ASSERT_EQ(tokens[2].text, "{");
    ASSERT_EQ(tokens[3].text, "}");
}

TEST(lexer, function_tokenization) {
    shade::compile_context context;
    constexpr const char * source = R"(
        func a(i32 b, i32 c) -> {
            return b + c;
        }
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "func");
    ASSERT_EQ(tokens[1].text, "a");
    ASSERT_EQ(tokens[2].text, "(");
    ASSERT_EQ(tokens[3].text, "i32");
    ASSERT_EQ(tokens[4].text, "b");
    ASSERT_EQ(tokens[5].text, ",");
    ASSERT_EQ(tokens[6].text, "i32");
    ASSERT_EQ(tokens[7].text, "c");
    ASSERT_EQ(tokens[8].text, ")");
    ASSERT_EQ(tokens[9].text, "->");
    ASSERT_EQ(tokens[10].text, "{");
    ASSERT_EQ(tokens[11].text, "return");
    ASSERT_EQ(tokens[12].text, "b");
    ASSERT_EQ(tokens[13].text, "+");
    ASSERT_EQ(tokens[14].text, "c");
    ASSERT_EQ(tokens[15].text, ";");
    ASSERT_EQ(tokens[16].text, "}");

}

TEST(lexer, special_symbols_with_various_spacing) {
    shade::compile_context context;
    constexpr const char * source = R"(
        { } + -   / * () \
        | &   $ ~ ? @ ; [] '
        ^ - , % < >~
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    for (auto & token : tokens) {
        ASSERT_TRUE(lex._special_symbols.contains(token.text.at(0)));
    }
}

TEST(lexer, delimiters) {
    shade::compile_context context;
    constexpr const char * source = "\t \r";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens.size(), 0);
}

TEST(lexer, extended_tokens) {
    shade::compile_context context;
    constexpr const char * source = R"(
        && &= || |= == <= <- >= != /* */ //
        /=  *= %= ^= [] ++ += -- -= #label :: ->
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    /* although the comment indicators are extended tokens, they are ignored in the output */

    ASSERT_EQ(tokens[0].text, "&&");
    ASSERT_EQ(tokens[1].text, "&=");
    ASSERT_EQ(tokens[2].text, "||");
    ASSERT_EQ(tokens[3].text, "|=");
    ASSERT_EQ(tokens[4].text, "==");
    ASSERT_EQ(tokens[5].text, "<=");
    ASSERT_EQ(tokens[6].text, "<-");
    ASSERT_EQ(tokens[7].text, ">=");
    ASSERT_EQ(tokens[8].text, "!=");
    ASSERT_EQ(tokens[9].text, "/=");
    ASSERT_EQ(tokens[10].text, "*=");
    ASSERT_EQ(tokens[11].text, "%=");
    ASSERT_EQ(tokens[12].text, "^=");
    ASSERT_EQ(tokens[13].text, "[]");
    ASSERT_EQ(tokens[14].text, "++");
    ASSERT_EQ(tokens[15].text, "+=");
    ASSERT_EQ(tokens[16].text, "--");
    ASSERT_EQ(tokens[17].text, "-=");
    ASSERT_EQ(tokens[18].text, "#label");
    ASSERT_EQ(tokens[19].text, "::");
    ASSERT_EQ(tokens[20].text, "->");

    ASSERT_EQ(tokens.size(), 21);
}

TEST(lexer, negative_number) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        -20
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "-");
    ASSERT_EQ(tokens[1].text, "20");
}


TEST(lexer, string_test) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        "this is some string value"
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "\"");
    ASSERT_EQ(tokens[1].text, "this is some string value");
    ASSERT_EQ(tokens[2].text, "\"");
}

TEST(lexer, string_test_multiline) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        "this is some
 string value"
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "\"");
    ASSERT_EQ(tokens[1].text, "this is some string value");
    ASSERT_EQ(tokens[2].text, "\"");
}

TEST(lexer, string_test_multiline_inline) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        "this is some \n string value"
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "\"");
    ASSERT_EQ(tokens[1].text, "this is some \n string value");
    ASSERT_EQ(tokens[2].text, "\"");
}

TEST(lexer, string_test_tab) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        "this is some \t string value"
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "\"");
    ASSERT_EQ(tokens[1].text, "this is some \t string value");
    ASSERT_EQ(tokens[2].text, "\"");
}

TEST(lexer, character) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        'c'
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, shade::cstring::from_u64('c'));
}

TEST(lexer, integer) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        100
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, shade::cstring::from_u64(100));
}

TEST(lexer, floating_point) {
    //this test checks that a negative number is correctly tokenized
    //the sign and number are separate tokens and will be correctly
    //used by the parser.
    shade::compile_context context;
    constexpr const char * source = R"(
        3.5
    )";

    auto lex = shade::lexer(source, context);
    const auto tokens = lex.tokenize();

    ASSERT_EQ(tokens[0].text, "3.5");
}


