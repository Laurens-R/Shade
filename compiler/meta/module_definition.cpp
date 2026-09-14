//
// Created by laure on 11/09/2026.
//

#include "module_definition.hpp"
#include "../lang/spelling.hpp"
#include "../lang/path_utils.hpp"

namespace shade {
    module_definition::module_definition()
        : _child_modules{std::make_unique<std::deque<module_definition> >()} {}

    module_definition::module_definition(const cstring &module_name, const cstring &module_path, module_definition * parent)
        : _child_modules{std::make_unique<std::deque<module_definition> >()}, name(module_name), full_path{module_path}, _parent_module{parent} {}

    auto module_definition::begin() {
        return _child_modules->begin();
    }

    auto module_definition::end() {
        return _child_modules->end();
    }

    auto module_definition::size()  {
        return _child_modules->size();
    }

    module_definition *module_definition::find_namespace_by_absolute_path(const cstring & path) {
        //first ensure we perform this function at the root module
        if (_parent_module != nullptr) {
            return _parent_module->module_definition::find_namespace_by_absolute_path(path);
        }

        return find_namespace(path);
    }

    module_definition *module_definition::find_namespace(const cstring &path) {
        if (path.is_empty()) {
            return this;
        }

        module_definition *current_module = this;
        auto path_parts = path_utils::get_parts_from_path(path);

        for (size_t current_depth = 0; current_depth < path_parts.size(); ++current_depth) {
            for (auto & mod : *current_module) {
                if (path_parts[current_depth] == mod.name) {
                    current_module = &mod;
                    break;
                }
            }

            if (current_module->full_path == path) {
                //success! We've found the correct module
                return current_module;
            }

            //we need to explore the remaining children
            if (current_module->size() > 0) {
                continue;
            }

            //uh-oh we ended up here. which means the path wasn't found
            //because either we would have jumped in the next child
            //or we've reached the end of the child modules without a
            //full match on the path
            return nullptr;
        }

        return nullptr;
    }

    module_definition * module_definition::find_namespace_by_relative_path(const cstring &relative_path) {
        //first check if the module can be found straight down from here.
        auto initial_module_result = find_namespace(relative_path);
        if (initial_module_result != nullptr) return initial_module_result;

        if (_parent_module == nullptr) {
            return nullptr;
        }

        module_definition * current_module = _parent_module;

        while (current_module != nullptr) {
            auto found_module = current_module->find_namespace(relative_path);
            if (found_module != nullptr) {
                return found_module;
            } else {
                current_module = current_module->_parent_module;
            }
        }

        return nullptr;
    }

    type_definition * module_definition::find_type_by_absolute_path(const cstring &path) {
        if (_parent_module != nullptr) return _parent_module->find_type_by_absolute_path(path);

        module_definition * found_module = this;

        auto parent_namespace = path_utils::get_parent_namespace_from_path(path);
        if (!parent_namespace.is_empty()) {
            found_module = find_namespace_by_absolute_path(parent_namespace);
            if (found_module == nullptr) return nullptr;
        }

        for (auto &type : found_module->_child_types) {
            if (type.full_path == path) {
                return &type;
            }
        }

        return nullptr;
    }

    type_definition * module_definition::find_type_by_relative_path(const cstring &path) {
        module_definition * found_module = this;

        auto parent_namespace = path_utils::get_parent_namespace_from_path(path);
        if (!parent_namespace.is_empty()) {
            found_module = find_namespace_by_relative_path(parent_namespace);
            if (found_module == nullptr) return nullptr;
        }

        for (auto &type : found_module->_child_types) {
            if (type.full_path == path) {
                return &type;
            }
        }

        return nullptr;
    }

    function_definition * module_definition::find_function_by_absolute_path(const cstring &path) {
        if (_parent_module != nullptr) return _parent_module->find_function_by_absolute_path(path);

        module_definition * found_module = nullptr;
        type_definition * found_type = nullptr;

        auto parent_namespace = path_utils::get_parent_namespace_from_path(path);
        if (!parent_namespace.is_empty()) {
            found_module = find_namespace_by_absolute_path(parent_namespace);
            if (found_module == nullptr) {
                found_type = find_type_by_absolute_path(parent_namespace);
                if (found_type == nullptr) {
                    return nullptr;
                }
            }
        }

        if (found_module != nullptr) {
            for (auto &child_function : found_module->_child_functions) {
                if (child_function.full_path == path) {
                    return &child_function;
                }
            }
        } else if (found_type != nullptr) {
            for (auto &child_method : found_type->methods) {
                if (child_method.full_path == path) {
                    return &child_method;
                }
            }
        }

        return nullptr;
    }

    function_definition * module_definition::find_function_by_relative_path(const cstring &path) {
        module_definition * found_module = this;

        auto parent_namespace = path_utils::get_parent_namespace_from_path(path);
        if (!parent_namespace.is_empty()) {
            found_module = find_namespace_by_relative_path(parent_namespace);
            if (found_module == nullptr) return nullptr;
        }

        for (auto &child_function : found_module->_child_functions) {
            if (child_function.full_path == path) {
                return &child_function;
            }
        }

        return nullptr;
    }

    module_definition *module_definition::add_child_module(const cstring &full_module_path) {

        auto module_path = path_utils::get_parent_namespace_from_path(full_module_path);

        if (!module_path.is_empty()) {
            auto found_parent_module = find_namespace_by_absolute_path(module_path);
            if (found_parent_module == nullptr) return nullptr;

            auto module_name = path_utils::get_object_from_path(full_module_path);

            found_parent_module->_child_modules->emplace_back(
                module_name, full_module_path, found_parent_module
            );

            return &found_parent_module->_child_modules->back();

        } else {
            //global scope module
            _child_modules->emplace_back(
                full_module_path, full_module_path, this
            );

            return &_child_modules->back();
        };
    }

    type_definition * module_definition::add_child_type(const type_definition &type) {
        auto module_path = type.module_path;
        auto found_module = find_namespace_by_absolute_path(module_path);

        found_module->_child_types.emplace_back(type);
        auto added_type = &found_module->_child_types.back();
        added_type->related_module = found_module;
        added_type->module_path = found_module->full_path;
        added_type->full_path = found_module->full_path + spelling::modules::module_seperator + type.name;
        return added_type;
    }

    function_definition * module_definition::add_child_function(const function_definition &func) {
        module_definition * found_module = nullptr;
        type_definition * found_type = nullptr;

        if (func.is_method) {
            auto module_path = path_utils::get_parent_namespace_from_path(func.parent_path);
            found_module = find_namespace_by_absolute_path(module_path);
            found_type = find_type_by_absolute_path(func.parent_path);
        } else {
            auto module_path = func.parent_path;
            found_module = find_namespace_by_absolute_path(module_path);
        }

        function_definition * added_function = nullptr;

        if (found_type == nullptr) {
            //if there is no found type, we add the function to the module
            found_module->_child_functions.emplace_back(func);
            added_function = &found_module->_child_functions.back();
            added_function->parent_path = found_module->full_path;

            added_function->full_path = path_utils::remove_global_prefix(found_module->full_path + spelling::modules::module_seperator + func.name);
        } else {
            //else we add the function to the type
            found_type->methods.emplace_back(func);
            added_function = &found_type->methods.back();
            added_function->parent_path = found_type->full_path;
            added_function->full_path = found_type->full_path + spelling::modules::module_seperator + func.name;
        }

        added_function->related_module = found_module;

        return added_function;
    }
} // shade
