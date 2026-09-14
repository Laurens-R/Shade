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
#include "../lang/namespace.hpp"

namespace shade {
    class type_index {
        private:
            std::vector<type_information*>           _registered_types;
            std::unordered_map<std::string, size_t>  _registered_type_mapping;

        public:
            module_definition global;

            type_index();

            std::span<type_information*> get_types();

            bool add_function_to_module(const cstring & module_path, const function_definition & func);

            bool add_function_to_struct(const cstring & struct_path, const function_definition & func);

            bool add_typeinformation(const type_information &typeinfo);

            bool contains_typeinformation(const cstring &type_path);

            type_information *get_typeinformation(const cstring &type_path);
    };
}


#endif //SHADE_TYPE_INDEX_H
