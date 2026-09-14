//
// Created by laure on 11/09/2026.
//

#include "type_index.h"
#include "../utils/exceptions.hpp"

namespace shade {
    type_index::type_index() {
        global.full_path = cstring::empty();
        global.name      = cstring::empty();

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

    std::span<type_information *> type_index::get_types() {
        return _registered_types;
    }

    bool type_index::add_function_to_module(const cstring &module_path, const function_definition &func) {
        if (module_path.is_empty()) {
            global.add_child_function(func);
        }

        module_definition * found_module = global.find_namespace_by_absolute_path(module_path);
        if (found_module == nullptr) return false;

        found_module->add_child_function(func);
        return true;
    }

    bool type_index::add_function_to_struct(const cstring &struct_path, const function_definition &func) {
        if (struct_path.is_empty()) return false;

        auto found_type = get_typeinformation(struct_path);
        if (found_type == nullptr) return false;

        found_type->add_method(func);

        return true;
    }

    bool type_index::add_typeinformation(const type_information &typeinfo) {
        if (contains_typeinformation(typeinfo.full_path)) {
            throw compiler_exception("Type already defined.");
        }

        auto module_path = namespace_path::get_parent(typeinfo.full_path);
        auto found_module = global.find_namespace_by_absolute_path(module_path);

        if (found_module == nullptr) {
            found_module = &global;
        }

        auto added_child_location = found_module->add_child_type(typeinfo);

        _registered_types.push_back(added_child_location);
        _registered_type_mapping[typeinfo.full_path.to_c_string()] = _registered_types.size() - 1;

        return true;
    }

    bool type_index::contains_typeinformation(const cstring &type_path) {
        return _registered_type_mapping.contains(type_path.to_c_string());
    }

    type_information * type_index::get_typeinformation(const cstring &type_path) {
        if (!contains_typeinformation(type_path)) return nullptr;
        return _registered_types[_registered_type_mapping.at(type_path.to_c_string())];
    }
}
