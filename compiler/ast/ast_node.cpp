//
// Created by laure on 11/09/2026.
//

#include "ast_node.hpp"
#include "../meta/type_definition.hpp"

namespace shade
{
    std::optional<type_definition*> ast_node::get_type_metadata(ast_node* parent)
    {
        if (metadata && metadata->get_type() == metadata_type::type)
        {
            return dynamic_cast<type_definition*>(metadata.get());
        }

        return std::nullopt;
    }

    std::optional<function_definition*> ast_node::get_function_metadata(ast_node* parent)
    {
        if (metadata && metadata->get_type() == metadata_type::function)
        {
            return dynamic_cast<function_definition*>(metadata.get());
        }

        return std::nullopt;
    }
} // shade