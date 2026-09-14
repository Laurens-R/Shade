//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_FUNCTION_H
#define SHADE_FUNCTION_H

#include <vector>

#include "../utils/cstring.hpp"

#include "attribute.hpp"
#include "metadata.hpp"

namespace shade
{
    struct type_definition;

    struct function_argument
    {
        cstring name;
        type_definition* type;
    };

    class ast_node;
    struct module_definition;
    struct capture_results;

    struct function_definition final : public metadata
    {
        std::vector<function_argument> arguments;
        std::vector<attribute> attributes;
        cstring name;
        cstring full_path;
        cstring module_path;

        ast_node* related_node = nullptr;
        module_definition* related_module = nullptr;
        capture_results * related_token_capture = nullptr;
        type_definition* related_type = nullptr;
        type_definition* return_type = nullptr;

        bool is_method = false;

        ~function_definition() override = default;

        metadata_type get_type() override;

        static function_definition create(const cstring &full_path, bool is_method);
    };
} // shade

#endif //SHADE_FUNCTION_H
