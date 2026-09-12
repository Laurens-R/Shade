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
            global.full_path = cstring::empty();
            global.name = cstring::empty();
            
            //prepopulate the type index with built-in primitive types.
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::boolean));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::u8));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::u16));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::u32));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::u64));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::i8));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::i16));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::i32));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::i64));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::f32));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::f64));
            add_typeinformation(type_information::get_for_primitive_type(primitive_types::string));
        }

        bool add_typeinformation(const type_information& typeinfo)
        {
            if (contains_typeinformation(typeinfo.get_full_path())) return false;;
            _registered_types.push_back(typeinfo);
            _registered_type_mapping[typeinfo.get_full_path().to_c_string()] = _registered_types.size() - 1;
            return true;
        }

        bool contains_typeinformation(const cstring & type_path) {
            return _registered_type_mapping.contains(type_path.to_c_string());
        }

        type_information * get_typeinformation(const cstring & type_path) {
            if (!contains_typeinformation(type_path)) return nullptr;
            return &_registered_types[_registered_type_mapping.at(type_path.to_c_string())];
        }
    };
}



#endif //SHADE_TYPE_INDEX_H
