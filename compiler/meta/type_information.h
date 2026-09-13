//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_TYPE_INFORMATION_H
#define SHADE_TYPE_INFORMATION_H

#include <expected>
#include <vector>

#include "function.h"
#include "metadata.h"

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

    struct type_information;

    struct type_field_definition {
        cstring           name                     = cstring::empty();
        type_information *related_type_information = {};
        size_t            offset                   = 0;
    };

    enum class generics_error {
        not_a_generic_type,
        target_type_does_not_exist,
        generic_count_mismatch,
        incomplete_type
    };

    class ast_node;

    struct type_information final : public metadata {
        std::vector<cstring>               generics;
        std::vector<type_field_definition> fields;
        std::vector<function>              methods;
        cstring                            name             = cstring::empty();
        cstring                            module_path      = cstring::empty();
        cstring                            full_path        = cstring::empty();
        primitive_types                    primitive_type   = primitive_types::unknown;
        reference_type                     reference_type   = reference_type::none;
        ast_node *                         related_node     = nullptr;
        uint64_t                           fixed_array_size = 0;
        uint8_t                            alignment        = sizeof(uintptr_t);
        bool                               is_primitive     = false;
        bool                               is_const         = false;
        bool                               is_struct        = false;
        bool                               is_enum          = false;
        bool                               is_generic_type  = false;
        bool                               is_array         = false;
        bool                               is_dynamic_array = false;

        ~type_information() = default;

        [[nodiscard]] size_t get_size() const;

        [[nodiscard]] uint64_t get_hash() const;

        void calculate_offsets();

        [[nodiscard]] bool are_fields_valid() const;

        void add_field(const cstring &field_name, type_information *type);

        void add_method(const cstring &method_name, ast_node *related_node, type_information *return_type, const std::vector<function_argument> &arguments);

        std::expected<type_information, generics_error> monomorphize(const std::vector<type_information *> types);

        static type_information get_for_primitive_type(primitive_types primitive_type);

        static type_information create_struct(const cstring &full_path, ast_node *related_node, size_t alignment = sizeof(uintptr_t));

        metadata_type get_type() override;
    };
}


#endif //SHADE_TYPE_INFORMATION_H
