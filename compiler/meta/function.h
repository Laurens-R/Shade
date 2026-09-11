//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_FUNCTION_H
#define SHADE_FUNCTION_H

#include <vector>

#include "../utils/cstring.hpp"

#include "attribute.h"
#include "metadata.h"

namespace shade
{
    struct type_information;

    struct function_argument
    {
        cstring name;
        type_information* type;
    };

    class ast_node;

    struct function final : public metadata
    {
        std::vector<function_argument> arguments;
        std::vector<attribute> attributes;
        cstring name;
        type_information* return_type;
        ast_node* related_node;
        bool is_method;

        ~function() override = default;

        metadata_type get_type() override;
    };
} // shade

#endif //SHADE_FUNCTION_H
