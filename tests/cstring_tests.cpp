//
// Created by Laurens Ruijtenberg on 11/09/2026.
//

#include <gtest/gtest.h>

#include <utils/cstring.hpp>

TEST(cstring, length) {
    shade::cstring str = "Hello, World!";
    ASSERT_EQ(str.length(), 13);
}

TEST(cstring, equality) {
    shade::cstring str = "Hello, World!";
    ASSERT_TRUE(str == "Hello, World!");
}

TEST(cstring, begins_with) {
    shade::cstring str = "Hello, World!";
    ASSERT_TRUE(str.begins_with("Hello,"));
}

TEST(cstring, ends_with) {
    shade::cstring str = "abcd";
    auto endswith = str.ends_with("cd");
    ASSERT_TRUE(endswith);
}

TEST(cstring, contains) {
    shade::cstring str = "Hello, World!";
    ASSERT_TRUE(str.contains(", World"));
}

TEST(cstring, replace) {
    shade::cstring str = "Hello, World!";
    ASSERT_EQ(str.replace("Hello", "Hi"), "Hi, World!");
}

TEST(cstring, replace_with_nothing) {
    shade::cstring str = "Hello, World!";
    ASSERT_EQ(str.replace("Hello, ", shade::cstring::empty()), "World!");
}

TEST(cstring, at){
    shade::cstring str = "Hello, World!";
    ASSERT_EQ(str.at(1), 'e');
}

TEST(cstring, is_true_bool){
    shade::cstring str = "true";
    ASSERT_TRUE(str.is_bool());
}

TEST(cstring, is_false_bool){
    shade::cstring str = "false";
    ASSERT_TRUE(str.is_bool());
}

TEST(cstring, is_float){
    shade::cstring str = "1.0";
    ASSERT_TRUE(str.is_float());
}

TEST(cstring, is_unsigned_int){
    shade::cstring str = "1";
    ASSERT_TRUE(str.is_unsigned_int());

    shade::cstring fvalue = "1.0";
    ASSERT_FALSE(fvalue.is_unsigned_int());
}

TEST(cstring, false_is_unsigned_int_with_float) {
    shade::cstring fvalue = "1.0";
    ASSERT_FALSE(fvalue.is_unsigned_int());
}

TEST(cstring, is_signed_int){
    shade::cstring str = "1";
    ASSERT_TRUE(str.is_signed_int());
}

TEST(cstring, is_negative_signed_int){
    shade::cstring str = "-1";
    ASSERT_TRUE(str.is_signed_int());
}

TEST(cstring, is_empty){
    shade::cstring str = shade::cstring::empty();
    ASSERT_TRUE(str.is_empty());
}

TEST(cstring, empty_literal){
    ASSERT_EQ(shade::cstring::empty(), "");
}






