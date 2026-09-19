//
// Created by laure on 11/09/2026.
//

#include "function_definition.hpp"

#include <compiler/lang/path_utils.hpp>
#include "compiler/exceptions.hpp"
#include "type_definition.hpp"

namespace shade
{
    function_definition function_definition::create(const cstring &full_path, bool is_method) {
        function_definition result;

        result.full_path = full_path;
        result.name = path_utils::get_object_from_path(full_path);
        result.parent_path = path_utils::get_parent_namespace_from_path(full_path);
        result.is_method = is_method;

        return result;
    }

    void function_definition::add_variable(variable_definition &var) {
        if (variables.empty()) {
            var.offset = 0;
        } else {
            auto & last_var = variables.back();
            auto last_offset = last_var.offset;

            if (var.type_declaration.type == 0) {
                throw compiler_exception("Internal compiler error: type is null while adding variable");
            }

            var.offset = last_offset +  var.type_declaration.type->get_size();
        }

        variables.push_back(var);
    }

    variable_definition * function_definition::find_variable(const cstring &name) {
        for (auto & variable : variables) {
            if (variable.name == name) {
                return &variable;
            }
        }

        return nullptr;
    }
} // shade