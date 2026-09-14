//
// Created by laure on 11/09/2026.
//

#include "function_definition.hpp"

#include "lang/path_utils.hpp"

namespace shade
{
    metadata_type function_definition::get_type()
    {
        return metadata_type::function;
    }

    function_definition function_definition::create(const cstring &full_path, bool is_method) {
        function_definition result;

        result.full_path = full_path;
        result.name = path_utils::get_object_from_path(full_path);
        result.parent_path = path_utils::get_parent_namespace_from_path(full_path);
        result.is_method = is_method;

        return result;
    }
} // shade