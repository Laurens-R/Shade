//
// Created by Laurens Ruijtenberg on 14/09/2026.
//
#include <gtest/gtest.h>

#include <meta/module_definition.hpp>


TEST(module_definition, find_namespace) {
    shade::module_definition m1("a", "a");
    auto m2 = m1.add_child_module("b");
    auto m3 = m1.add_child_module("c");
    m2->add_child_module("d");
    m3->add_child_module("e");

    auto found_module = m1.find_namespace_by_absolute_path("a::c::e");

    ASSERT_NE(found_module, nullptr);
    ASSERT_EQ(found_module->full_path, "a::c::e");
    ASSERT_EQ(found_module->name, "e");
}