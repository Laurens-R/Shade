//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_FUNCTION_H
#define SHADE_FUNCTION_H

#include <vector>

#include "../../shared/utils/cstring.hpp"

#include "attribute.hpp"
#include "type_expression.hpp"
#include "variable_definition.hpp"

namespace shade
{
    struct type_definition;

    struct function_argument
    {
        cstring name;
        type_expression type;
    };

    class ast_node;
    struct module_definition;
    struct parsed_ranges;

    struct function_definition final
    {
        std::vector<function_argument> arguments;
        std::vector<attribute> attributes;
        std::vector<variable_definition> variables;
        cstring name;
        cstring full_path;
        cstring parent_path;

        ast_node* related_node = nullptr;
        module_definition* related_module = nullptr;
        parsed_ranges * related_token_capture = nullptr;
        type_definition* related_type = nullptr;
        type_expression return_type{};

        bool has_return_type = false;
        bool is_method = false;

        static function_definition create(const cstring &full_path, bool is_method);

        void add_variable(variable_definition &var);
        variable_definition *find_variable(const cstring &name);
       
    };
} // shade

#endif //SHADE_FUNCTION_H
