//
// Created by laure on 11/09/2026.
//

#include "module_definition.hpp"
#include "../lang/spelling.hpp"
#include "../lang/namespace.hpp"

namespace shade
{
    module_definition * module_definition::find_namespace(const cstring& namespace_path)
    {
        if (namespace_path.is_empty()) return nullptr;

        if (namespace_path::is_part_of(namespace_path, full_path))
        {
            cstring relative_path = namespace_path::get_relative_path(namespace_path, full_path);
            auto remaining_parts = namespace_path::get_parts(relative_path);

            if (remaining_parts.empty())
            {
                return nullptr;
            }

            auto & next_module = remaining_parts.front();

            for (auto & module : child_modules)
            {
                if (module.name == next_module)
                {
                    if (remaining_parts.size() == 1)
                    {
                        return &module;
                    }
                    else
                    {
                        //TEST TEST TEST
                        auto smaller_path_parts = std::vector<cstring>(remaining_parts.begin() + 1, remaining_parts.end());
                        cstring next_layer_path = namespace_path::get_path_from_parts(smaller_path_parts);
                        return find_namespace(next_layer_path);
                    }
                }
            }
        }

        return nullptr;
    }

    void module_definition::add_child_module(const cstring & child_name)
    {
        child_modules.push_back( {
            .name = child_name,
            .full_path = full_path + spelling::modules::module_seperator + name
        });
    }

    void module_definition::add_type(const type_information& type)
    {
        child_types.push_back(type);
    }
} // shade