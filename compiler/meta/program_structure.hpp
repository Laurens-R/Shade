//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_TYPE_INDEX_H
#define SHADE_TYPE_INDEX_H

#include <vector>
#include <unordered_map>
#include <string>
#include <span>

#include "module_definition.hpp"
#include "../lang/path_utils.hpp"

namespace shade {
    class program_structure {
        private:
            std::vector<type_definition*>           _registered_types;
            std::vector<function_definition*>        _registered_functions;
            std::vector<module_definition*>          _registered_modules;
            std::unordered_map<std::string, size_t>  _registered_type_mapping;
            std::unordered_map<std::string, size_t>  _registered_function_mapping;
            std::unordered_map<std::string, size_t>  _registered_module_mapping;

            module_definition _global_module;
        public:

            program_structure();

            std::span<type_definition*> get_types();

            module_definition *add_module(const cstring &name);

            function_definition *add_function_to_module(const cstring &module_path, const function_definition &func);

            function_definition *add_function_to_type(const cstring &type_path, const function_definition &func);

            bool add_typeinformation(const type_definition &typeinfo);

            bool contains_typeinformation(const cstring &type_path);

            type_definition *get_typeinformation(const cstring &type_path);

            module_definition * get_global_module();
    };
}


#endif //SHADE_TYPE_INDEX_H
