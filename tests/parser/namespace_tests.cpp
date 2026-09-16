//
// Created by Laurens Ruijtenberg on 11/09/2026.
//

#include <gtest/gtest.h>

#include <compiler/lang/path_utils.hpp>

TEST(namespaces, global_path) {
    auto global_path = shade::path_utils::get_global_path();
    ASSERT_EQ(global_path, shade::cstring::empty());
}

TEST(namespaces, part_splitting) {
    auto parts = shade::path_utils::get_parts_from_path("a::b::c");
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "a");
    ASSERT_EQ(parts[1], "b");
    ASSERT_EQ(parts[2], "c");
}

TEST(namespaces, part_splitting_inc_global) {
    auto parts = shade::path_utils::get_parts_from_path("::a::b::c");
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "a");
    ASSERT_EQ(parts[1], "b");
    ASSERT_EQ(parts[2], "c");
}

TEST(namespaces, get_type_from_path) {
    auto typestr = shade::path_utils::get_object_from_path("a::b::c");
    ASSERT_EQ(typestr, "c");
}

TEST(namespaces, split_merged) {
    auto parts = shade::path_utils::get_parts_from_path("a::b::c");
    auto merged = shade::path_utils::get_path_from_parts(parts);
    ASSERT_EQ(merged, "a::b::c");
}

TEST(namespaces, get_relative_path) {
    auto relativepath = shade::path_utils::get_relative_path("a::b::c", "a::b");
    ASSERT_EQ(relativepath, "c");
}

TEST(namespaces, get_relative_path_inc_global) {
    auto relativepath = shade::path_utils::get_relative_path("a::b::c", "::a::b");
    ASSERT_EQ(relativepath, "c");
}

TEST(namespaces, is_part_of) {
    auto is_part = shade::path_utils::is_part_of("a::b::c", "a::b");
    ASSERT_TRUE(is_part);
}

