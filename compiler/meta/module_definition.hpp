//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_MODULE_H
#define SHADE_MODULE_H

#include <deque>
#include <span>

#include "../utils/cstring.hpp"
#include "type_information.h"
#include "function_definition.h"

namespace shade {
    struct capture_results;

    struct module_definition {
        protected:
            module_definition * _parent_module;
            std::unique_ptr<std::deque<module_definition>> _child_modules;
            std::deque<type_information> _child_types;
            std::deque<function_definition> _child_functions;

            module_definition *find_namespace(const cstring &path);
        public:

            cstring name;
            cstring full_path;
            capture_results * related_token_capture = nullptr;

            module_definition();
            module_definition(const cstring &module_name, const cstring &module_path, module_definition * parent = nullptr);

            [[nodiscard]] auto begin();
            [[nodiscard]] auto end();
            [[nodiscard]] auto size();

            module_definition *find_namespace_by_absolute_path(const cstring &path);

            module_definition *find_namespace_by_relative_path(const cstring &relative_path);

            type_information * find_type_by_absolute_path(const cstring & path);

            type_information * find_type_by_relative_path(const cstring & path);

            module_definition *add_child_module(const cstring &child_name);

            type_information *add_child_type(const type_information &type);

            function_definition *add_child_function(const function_definition &func);
    };
} // shade

#endif //SHADE_MODULE_H
