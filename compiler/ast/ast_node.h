//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_AST_NODE_H
#define SHADE_AST_NODE_H

#include <memory>
#include <optional>
#include <vector>


#include "../utils/cstring.hpp"
#include "../lexer/lexer.hpp"
#include "../meta/metadata.h"

namespace shade
{

    struct type_information;
    struct function;

    struct ast_node
    {
        ast_node * parent;
        std::vector<std::shared_ptr<ast_node>> children;
        std::shared_ptr<metadata> metadata;
        language_token token;

        ast_node * create_child(ast_node * parent);
        std::optional<type_information*> get_type_metadata(ast_node * parent);
        std::optional<function*> get_function_metadata(ast_node * parent);



    };
} // shade

#endif //SHADE_AST_NODE_H
