//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_TYPE_INDEX_H
#define SHADE_TYPE_INDEX_H

#include <vector>
#include <unordered_map>

#include "module_definition.hpp"
#include "../lang/namespace.hpp"

namespace shade
{
    class type_index {
    private:
        std::vector<type_information> _registered_types;
        std::unordered_map<const char*, size_t> _registered_type_mapping;
    public:

        module_definition global;

        type_index()
        {
            global.full_path = namespace_path::get_global_path();
            global.name = "::";
        }
    };
}



#endif //SHADE_TYPE_INDEX_H
