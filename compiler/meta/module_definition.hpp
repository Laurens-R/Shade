//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_MODULE_H
#define SHADE_MODULE_H

#include <vector>

#include "../utils/cstring.hpp"
#include "type_information.h"

namespace shade
{
    struct module_definition
    {
    private:
        module_definition * find_next_namespace(const cstring & namespace_path);
    public:
        std::vector<module_definition> child_modules;
        std::vector<type_information> child_types;

        cstring name;
        cstring full_path;

        module_definition * find_namespace(const cstring & namespace_path);

        module_definition * add_child_module(const cstring &child_name);
        void add_type(const type_information & type);
    };
} // shade

#endif //SHADE_MODULE_H
