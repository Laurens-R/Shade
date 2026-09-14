//
// Created by Laurens Ruijtenberg on 14/09/2026.
//
#include <gtest/gtest.h>

#include <meta/module_definition.hpp>
#include <meta/function_definition.hpp>

TEST(module_definition, find_namespace) {
    shade::module_definition global_module("a", "a");
    global_module.add_child_module("a::b");
    global_module.add_child_module("a::c");
    global_module.add_child_module("a::b::d");
    global_module.add_child_module("a::c::e");

    auto found_module = global_module.find_namespace_by_absolute_path("a::c::e");

    ASSERT_NE(found_module, nullptr);
    ASSERT_EQ(found_module->full_path, "a::c::e");
    ASSERT_EQ(found_module->name, "e");
}

TEST(module_definition, global_function) {
    shade::module_definition global_module("a", "a");
    shade::function_definition f1 = shade::function_definition::create("a::f1", false);
    global_module.add_child_function(f1);

    auto found_function = global_module.find_function_by_absolute_path("a::f1");

    ASSERT_NE(found_function, nullptr);
    ASSERT_EQ(found_function->full_path, "a::f1");
    ASSERT_EQ(found_function->name, "f1");
}