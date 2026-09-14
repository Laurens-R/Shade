//
// Created by laure on 11/09/2026.
//

#include "function_definition.h"

#include "lang/namespace.hpp"

namespace shade
{
    metadata_type function_definition::get_type()
    {
        return metadata_type::function;
    }

    function_definition function_definition::create(const cstring &full_path, bool is_method) {
        function_definition result;

        result.full_path = full_path;
        result.name = namespace_path::get_type_from_path(full_path);
        result.module_path = namespace_path::get_parent(full_path);
        result.is_method = is_method;

        return result;
    }
} // shade