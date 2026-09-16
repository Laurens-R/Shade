//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_FUNCTION_H
#define SHADE_FUNCTION_H

#include <vector>

#include "../../shared/utils/cstring.hpp"

#include "attribute.hpp"
#include "metadata.hpp"

namespace shade
{
    struct type_definition;
    struct type_expression;

    struct function_argument
    {
        cstring name;
        type_expression* type;
    };

    class ast_node;
    struct module_definition;
    struct parsed_ranges;

    struct function_definition final : public metadata
    {
        std::vector<function_argument> arguments;
        std::vector<attribute> attributes;
        cstring name;
        cstring full_path;
        cstring parent_path;

        ast_node* related_node = nullptr;
        module_definition* related_module = nullptr;
        parsed_ranges * related_token_capture = nullptr;
        type_definition* related_type = nullptr;
        type_definition* return_type = nullptr;

        bool is_method = false;

        ~function_definition() override = default;

        metadata_type get_type() override;

        static function_definition create(const cstring &full_path, bool is_method);
    };
} // shade

#endif //SHADE_FUNCTION_H
