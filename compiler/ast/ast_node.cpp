//
// Created by laure on 11/09/2026.
//

#include "ast_node.hpp"

namespace shade
{
    bool ast_node::has_arguments() const {
        return argument_nodes != nullptr;
    }

    bool ast_node::has_child_nodes() const {
        return children != nullptr;
    }

    void ast_node::ensure_child_nodes() {
        if (children == nullptr)
            children = std::make_unique<std::vector<ast_node>>();
    }

    void ast_node::ensure_argument_nodes() {
        if (argument_nodes == nullptr)
            argument_nodes = std::make_unique<std::vector<ast_node>>();
    }

    void ast_node::set_left_node(const ast_node & node) {
        ensure_child_nodes();

        if (children->size() != 2) {
            children->resize(2);
            is_binary_node = true;
        }

        (*children)[0] = node;
    }

    void ast_node::set_right_node(const ast_node & node) {
        ensure_child_nodes();

        if (children->size() != 2) {
            children->resize(2);
            is_binary_node = true;
        }

        (*children)[1] = node;
    }

    ast_node *ast_node::get_left_node() {
        ensure_child_nodes();

        if (is_binary_node)
            return &children->at(0);
        return nullptr;
    }

    ast_node *ast_node::get_right_node() {
        ensure_child_nodes();

        if (is_binary_node)
            return &children->at(1);
        return nullptr;
    }

    void ast_node::append_child_nodes(const std::vector<ast_node> &nodes) {
        ensure_child_nodes();
        children->append_range(nodes);
    }

    void ast_node::add_child_node(const ast_node &node) {
        ensure_child_nodes();
        children->push_back(node);
    }

    ast_node & ast_node::get_child_node(size_t index) const {
        return children->at(index);
    }

    size_t ast_node::get_child_node_count() const {
        if (children == nullptr) return 0;
        return children->size();
    }

    void ast_node::add_argument_node(const ast_node &node) {
        ensure_argument_nodes();
        argument_nodes->push_back(node);
    }

    ast_node & ast_node::get_argument_node(size_t index) const {
        return argument_nodes->at(index);
    }

    size_t ast_node::get_argument_node_count() const {
        if (argument_nodes == nullptr) return 0;
        return argument_nodes->size();
    }
} // shade