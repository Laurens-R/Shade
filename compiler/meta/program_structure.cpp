//
// Created by laure on 11/09/2026.
//

#include "program_structure.hpp"
#include "../exceptions.hpp"

namespace shade {
    program_structure::program_structure() {
        _global_module.full_path = cstring::empty();
        _global_module.name      = cstring::empty();

        //prepopulate the type index with built-in primitive types.
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::boolean));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::u8));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::u16));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::u32));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::u64));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::i8));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::i16));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::i32));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::i64));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::f32));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::f64));
        add_type_definition(type_definition::get_for_primitive_type(primitive_types::string));
    }

    std::span<type_definition *> program_structure::get_types() {
        return _registered_types;
    }

    std::span<function_definition *> program_structure::get_functions() {
        return _registered_functions;
    }

    std::span<module_definition *> program_structure::get_modules() {
        return _registered_modules;
    }

    module_definition *program_structure::add_module_definition(const cstring &name) {
        if (_registered_module_mapping.contains(name.to_c_string())) {
            throw compiler_exception("Module already defined.");
        }

        auto added_module = _global_module.add_child_module(name);
        if (added_module == nullptr) return nullptr;

        _registered_modules.push_back(added_module);
        _registered_module_mapping[name.to_c_string()] = _registered_modules.size() - 1;

        return added_module;
    }

    function_definition *program_structure::add_function_to_module(const cstring &module_path, const function_definition &func) {
        function_definition * added_function = nullptr;
        module_definition * found_module = nullptr;

        if (module_path.is_empty()) {
            found_module = &_global_module;
        } else {
            found_module = _global_module.find_namespace_by_absolute_path(module_path);
        }

        if (found_module == nullptr) return nullptr;

        added_function = found_module->add_child_function(func);
        if (added_function == nullptr) return nullptr;
        auto full_path = added_function->full_path;

        _registered_functions.push_back(added_function);
        _registered_function_mapping[full_path.to_c_string()] = _registered_functions.size() - 1;
        return added_function;
    }

    function_definition *program_structure::add_function_to_type(const cstring &type_path, const function_definition &func) {
        if (type_path.is_empty()) return nullptr;

        auto found_type = get_type_definition(type_path);
        if (found_type == nullptr) return nullptr;

        auto added_method = found_type->add_method(func);
        if (added_method == nullptr) return nullptr;
        auto full_path = added_method->full_path;

        _registered_functions.push_back(added_method);
        _registered_function_mapping[full_path.to_c_string()] = _registered_functions.size() - 1;
        return added_method;
    }

    type_definition *program_structure::add_type_definition(const type_definition &typeinfo) {
        if (contains_type_definition(typeinfo.full_path)) {
            throw compiler_exception("Type already defined.");
        }

        auto module_path = path_utils::get_parent_namespace_from_path(typeinfo.full_path);
        auto found_module = _global_module.find_namespace_by_absolute_path(module_path);

        if (found_module == nullptr) {
            found_module = &_global_module;
        }

        auto added_child = found_module->add_child_type(typeinfo);

        _registered_types.push_back(added_child);
        _registered_type_mapping[typeinfo.full_path.to_c_string()] = _registered_types.size() - 1;

        return added_child;
    }

    bool program_structure::contains_type_definition(const cstring &type_path) {
        return _registered_type_mapping.contains(type_path.to_c_string());
    }

    bool program_structure::contains_function_definition(const cstring &function_path) {
        return _registered_function_mapping.contains(function_path.to_c_string());
    }

    bool program_structure::contains_module_definition(const cstring &module_path) {
        return _registered_module_mapping.contains(module_path.to_c_string());
    }

    function_definition * program_structure::find_function_definition(const cstring &relative_path, const cstring &module_start_path) {
        module_definition * mod_def = get_module_definition(module_start_path);

        if (mod_def == nullptr) return nullptr;

        return mod_def->find_function_by_relative_path(relative_path);
    }

    type_definition * program_structure::find_type_definition(const cstring &relative_path, const cstring &module_start_path) {
        module_definition * mod_def = get_module_definition(module_start_path);

        if (mod_def == nullptr) return nullptr;

        return mod_def->find_type_by_relative_path(relative_path);
    }

    type_definition * program_structure::get_type_definition(const cstring &type_path) {
        if (!contains_type_definition(type_path)) return nullptr;
        return _registered_types[_registered_type_mapping.at(type_path.to_c_string())];
    }

    function_definition * program_structure::get_function_definition(const cstring &function_path) {
        if (!_registered_function_mapping.contains(function_path.to_c_string())) return nullptr;
        return _registered_functions[_registered_function_mapping.at(function_path.to_c_string())];
    }

    module_definition * program_structure::get_module_definition(const cstring &module_path) {
        if (module_path.is_empty()) return &_global_module;
        if (!_registered_module_mapping.contains(module_path.to_c_string())) return nullptr;
        return _registered_modules[_registered_module_mapping.at(module_path.to_c_string())];
    }

    module_definition * program_structure::get_global_module() {
        return &_global_module;
    }
}
