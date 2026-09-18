//
// Created by laure on 11/09/2026.
//

#include "ast_node.hpp"
#include "../meta/type_definition.hpp"

namespace shade
{
    void ast_node::create_child(const ast_node_types type)
    {
        ast_node new_child;
        new_child.parent = this;
        new_child.type = type;
        children.push_back(new_child);
    }

    void ast_node::set_left_node(const ast_node & node) {
        if (children.size() != 2) {
            children.resize(2);
            is_binary_node = true;
        }

        children[0] = node;
    }

    void ast_node::set_right_node(const ast_node & node) {
        if (children.size() != 2) {
            children.resize(2);
            is_binary_node = true;
        }

        children[1] = node;
    }

    ast_node *ast_node::get_left_node() {
        if (is_binary_node)
            return &children[0];
        return nullptr;
    }

    ast_node *ast_node::get_right_node() {
        if (is_binary_node)
            return &children[1];
        return nullptr;
    }
} // shade