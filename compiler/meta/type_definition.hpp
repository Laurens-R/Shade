//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_TYPE_INFORMATION_H
#define SHADE_TYPE_INFORMATION_H

#include <expected>
#include <vector>

#include "function_definition.hpp"
#include "metadata.hpp"

#include "../utils/cstring.hpp"
#include "../vendor/xxhash/xxhash.hpp"

namespace shade {
    enum class primitive_types {
        boolean,
        i8,
        i16,
        i32,
        i64,
        u8,
        u16,
        u32,
        u64,
        f32,
        f64,
        string,
        nothing,
        unknown
    };

    enum class reference_type {
        reference, pointer, none
    };

    struct type_definition;

    struct type_field_definition {
        cstring name = cstring::empty();
        type_definition *related_type_information = {};
        size_t offset = 0;
    };

    struct enum_entry {
        cstring name = cstring::empty();
        cstring value = cstring::empty();
    };

    enum class generics_error {
        not_a_generic_type,
        target_type_does_not_exist,
        generic_count_mismatch,
        incomplete_type
    };

    class ast_node;
    struct module_definition;
    struct parsed_ranges;

    /**
     * A type_definition represents any type in the language. Even primitive types.
     */
    struct type_definition final : public metadata {

        /**
         * All generics that have been specified as potential arguments for the type.
         */
        std::vector<cstring> generics;

        /**
         * All the fields that belong to the type.
         */
        std::vector<type_field_definition> fields;

        /**
         * All the methods that belong to the type.
         */
        std::vector<function_definition> methods;

        /**
         * All the enum entries that belong to the type.
         */
        std::vector<enum_entry> enum_entries;

        /**
         * The name of the type without the path. (eg: file)
         */
        cstring name = cstring::empty();

        /**
         * The path to the module that contains the type. (eg: std::io)
         */
        cstring module_path = cstring::empty();

        /**
         * The full path to the type (eg: std::io::file)
         */
        cstring full_path = cstring::empty();

        /**
         * If the type is a primitive type, this fields defines which primitive type it is.
         */
        primitive_types primitive_type = primitive_types::unknown;

        /**
         * If the field is either a reference or a pointer, this field defines which one it is.
         */
        reference_type reference_type = reference_type::none;

        /**
         * The ast_node that represents this type.
         */
        ast_node *related_node = nullptr;

        /**
         * The module_definition to which this type belongs.
         */
        module_definition *related_module = nullptr;

        /**
         * The parsed_ranges instance that represents the parsed tokens for this type.
         */
        parsed_ranges * related_token_capture = nullptr;

        /**
         * The alignment of the type.
         */
        uint8_t alignment = sizeof(uintptr_t);

        /**
         * Whether the type is a primitive type.
         */
        bool is_primitive = false;

        /**
         * Whether the type is a struct type.
         */
        bool is_struct = false;

        /**
         * Whether the type is an enum type. enum_entries will contain all the enum entries that belong to the type.
         */
        bool is_enum = false;

        /**
         * Whether the type contains generic arguments an thus needs to be monomorphized.
         */
        bool is_generic_type = false;

        ~type_definition() = default;

        /**
         * Calculates the size of the type, taking alignment into account.
         * @return The size of the type.
         */
        [[nodiscard]] size_t get_size() const;

        /**
         * Calculates the hash of the type.
         * @return The hash of the type.
         */
        [[nodiscard]] uint64_t get_hash() const;

        /**
         * (Re)Calculates the offsets of the fields of the type, taking the alignment into account.
         */
        void calculate_offsets();

        /**
         * Checks if the type in its current configuration is a valid type definition
         * @return True if the type is valid, false otherwise.
         */
        [[nodiscard]] bool are_fields_valid() const;

        /**
         * Adds a field to the type definition.
         * @param field_name The name of the field.
         * @param type The type of the field.
         */
        void add_field(const cstring &field_name, type_definition *type);

        /**
         * Adds a method to the type definition.
         * @param func_def The function definition of the method.
         * @return A pointer to the added method.
         */
        function_definition *add_method(const function_definition &func_def);

        /**
         * Monomorphizes the type definition based on the provided types.
         * @param types The types to use for monomorphization.
         * @return The monomorphized type definition.
         */
        std::expected<type_definition, generics_error> monomorphize(const std::vector<type_definition *> types);

        /**
         * Creates a type definition for a primitive type.
         * @param primitive_type The primitive type.
         * @return The type definition for the primitive type.
         */
        static type_definition get_for_primitive_type(primitive_types primitive_type);

        /**
         * Creates a type definition for a struct.
         * @param full_path The full path of the struct.
         * @param related_node The related AST node.
         * @param alignment The alignment of the struct.
         * @return The type definition for the struct.
         */
        static type_definition create(const cstring &full_path, ast_node *related_node = nullptr, size_t alignment = sizeof(uintptr_t));

        /**
         * Returns the type of the type definition. (Required to make decisions during parsing)
         * @return The type of the type definition.
         */
        metadata_type get_type() override;
    };

    /**
     * A type_expression represents every type of type indicator in the language.
     * No matter if it's related to a funtion argument, a variable declaration, a struct field
     * or any other place where a type is defined on one shape or the other.
     */
    struct type_expression {
        type_definition *type = nullptr;
        uint64_t fixed_array_size = 0;
        bool is_owned = false;
        bool is_const = false;
        bool is_array = false;
        bool is_ptr = false;
        bool is_ref = false;
        bool is_dynamic_array = false;
    };
}


#endif //SHADE_TYPE_INFORMATION_H
