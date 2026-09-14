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

    enum class generics_error {
        not_a_generic_type,
        target_type_does_not_exist,
        generic_count_mismatch,
        incomplete_type
    };

    class ast_node;
    struct module_definition;
    struct capture_results;

    struct type_definition final : public metadata {
        std::vector<cstring> generics;
        std::vector<type_field_definition> fields;
        std::vector<function_definition> methods;
        cstring name = cstring::empty();
        cstring module_path = cstring::empty();
        cstring full_path = cstring::empty();
        primitive_types primitive_type = primitive_types::unknown;
        reference_type reference_type = reference_type::none;
        ast_node *related_node = nullptr;
        module_definition *related_module = nullptr;
        capture_results * related_token_capture = nullptr;
        uint64_t fixed_array_size = 0;
        uint8_t alignment = sizeof(uintptr_t);
        bool is_primitive = false;
        bool is_const = false;
        bool is_struct = false;
        bool is_enum = false;
        bool is_generic_type = false;
        bool is_array = false;
        bool is_dynamic_array = false;

        ~type_definition() = default;

        [[nodiscard]] size_t get_size() const;

        [[nodiscard]] uint64_t get_hash() const;

        void calculate_offsets();

        [[nodiscard]] bool are_fields_valid() const;

        void add_field(const cstring &field_name, type_definition *type);

        function_definition *add_method(const function_definition &func_def);

        std::expected<type_definition, generics_error> monomorphize(const std::vector<type_definition *> types);

        static type_definition get_for_primitive_type(primitive_types primitive_type);

        static type_definition create_struct(const cstring &full_path, ast_node *related_node, size_t alignment = sizeof(uintptr_t));

        metadata_type get_type() override;
    };
}


#endif //SHADE_TYPE_INFORMATION_H
