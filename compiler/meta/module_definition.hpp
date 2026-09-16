//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_MODULE_H
#define SHADE_MODULE_H

#include <deque>
#include <span>

#include "../../shared/utils/cstring.hpp"
#include "type_definition.hpp"
#include "function_definition.hpp"

namespace shade {
    struct parsed_ranges;

    struct module_definition {
        protected:
            module_definition * _parent_module = nullptr;
            std::unique_ptr<std::deque<module_definition>> _child_modules;
            std::deque<type_definition> _child_types;
            std::deque<function_definition> _child_functions;

            module_definition *find_namespace(const cstring &path);
        public:

            cstring name;
            cstring full_path;
            parsed_ranges * related_token_capture = nullptr;

            module_definition();
            module_definition(const cstring &module_name, const cstring &module_path, module_definition * parent = nullptr);

            [[nodiscard]] auto begin();
            [[nodiscard]] auto end();
            [[nodiscard]] auto size();

            module_definition *find_namespace_by_absolute_path(const cstring &path);

            module_definition *find_namespace_by_relative_path(const cstring &relative_path);

            type_definition * find_type_by_absolute_path(const cstring & path);

            type_definition * find_type_by_relative_path(const cstring & path);

            function_definition * find_function_by_absolute_path(const cstring & path);

            function_definition * find_function_by_relative_path(const cstring & path);

            module_definition *add_child_module(const cstring &full_module_path);

            type_definition *add_child_type(const type_definition &type);

            function_definition *add_child_function(const function_definition &func);
    };
} // shade

#endif //SHADE_MODULE_H
