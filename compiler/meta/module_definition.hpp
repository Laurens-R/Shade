//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_MODULE_H
#define SHADE_MODULE_H

#include <deque>

#include "../utils/cstring.hpp"
#include "type_information.h"

namespace shade {
    struct module_definition {
        protected:
            std::unique_ptr<std::deque<module_definition>> _child_modules;

            module_definition *find_next_namespace(const cstring &namespace_path);

        public:
            module_definition();

            module_definition(const cstring &module_name, const cstring &module_path);

            cstring name;
            cstring full_path;

            module_definition *find_namespace(const cstring &namespace_path);
            module_definition *add_child_module(const cstring &child_name);
    };
} // shade

#endif //SHADE_MODULE_H
