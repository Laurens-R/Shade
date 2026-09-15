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

            /**
             * Returns all the currently known types.
             * @return a span of type definitions
             */
            std::span<type_definition*> get_types();

            /**
             * Returns all the currently known functions.
             * @return a span of function definitions
             */
            std::span<function_definition*> get_functions();

            /**
             * Returns all the currently known modules.
             * @return a span of module definitions
             */
            std::span<module_definition*> get_modules();

            /**
             * Add's a function definition to the program structure under the specified module.
             * @param module_path The complete path to the module (eg: "std::io")
             * @param func The function definition to add
             * @return A pointer to the added function definition.
             */
            function_definition *add_function_to_module(const cstring &module_path, const function_definition &func);

            /**
             * Add a function definition to a specific type (aka struct).
             * @param type_path The complete path to the type (eg: "std::io::file")
             * @param func The function definition to add.
             * @return A pointer to the added function definition.
             */
            function_definition *add_function_to_type(const cstring &type_path, const function_definition &func);

            /**
             * Add a module to the program structure.
             * @param name The name of the module to add in the form of a complete path (eg: "std::io").
             * @return A pointer to the added module definition.
             */
            module_definition *add_module_definition(const cstring &name);

            /**
             * Add a type definition to the program structure.
             * @param typeinfo The type definition to add. Ensure the full_path is correct as it will determine the location of the type in the program structure.
             * @return A pointer to the added type definition.
             */
            type_definition *add_type_definition(const type_definition &typeinfo);

            /**
             * Checks if a type at the specified path exists
             * @param type_path The path to the type (eg: "std::io::file")
             * @return True if it exists.
             */
            bool contains_type_definition(const cstring &type_path);

            /**
             * Checks if a function at the specified path exists (can be both inside of a module or a type).
             * @param function_path The path to the function (eg: "std::io::file::read")
             * @return True if it exists.
             */
            bool contains_function_definition(const cstring &function_path);

            /**
             * Checks if a module at the specified path exists.
             * @param module_path The path to the module (eg: "std::io")
             * @return True if it exists.
             */
            bool contains_module_definition(const cstring &module_path);

            /**
             * Tries to find a type definition at the specified path relative to the specified module path.
             * @param type_path The path to the type as stated in the code (eg: "string_builder")
             * @param module_path The path of the module where the type is being requested (eg: "std::io")
             * @return The type definition if it exists, nullptr otherwise.
             */
            type_definition * find_type_definition(const cstring &type_path, const cstring & module_path);

            /**
             * Tries to get a type definition at the specified path.
             * @param type_path The path to the type (eg: "string_builder")
             * @return The type definition if it exists, nullptr otherwise.
             */
            type_definition *get_type_definition(const cstring &type_path);

            /**
             * Tries to get a function definition at the specified path.
             * @param function_path The path to the function (eg: "string_builder")
             * @return The function definition if it exists, nullptr otherwise.
             */
            function_definition *get_function_definition(const cstring &function_path);

            /**
             * Tries to get a module definition at the specified path.
             * @param module_path The path to the module (eg: "std::io")
             * @return The module definition if it exists, nullptr otherwise.
             */
            module_definition *get_module_definition(const cstring &module_path);

            /**
             * Gets the global module.
             * @return The global module.
             */
            module_definition * get_global_module();



    };
}


#endif //SHADE_TYPE_INDEX_H
