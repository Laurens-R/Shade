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

            function_definition *add_function_to_module(const cstring &module_path, const function_definition &func);

            function_definition *add_function_to_type(const cstring &type_path, const function_definition &func);

            module_definition *add_module_definition(const cstring &name);

            type_definition *add_type_definition(const type_definition &typeinfo);

            bool contains_type_definition(const cstring &type_path);

            bool contains_function_definition(const cstring &function_path);

            bool contains_module_definition(const cstring &module_path);

            type_definition *get_type_definition(const cstring &type_path);

            function_definition *get_function_definition(const cstring &function_path);

            module_definition *get_module_definition(const cstring &module_path);

            module_definition * get_global_module();
    };
}


#endif //SHADE_TYPE_INDEX_H
