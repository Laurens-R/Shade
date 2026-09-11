//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_TYPE_INFORMATION_H
#define SHADE_TYPE_INFORMATION_H

#include <expected>
#include <vector>

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
        type_information* related_type_information = {};
        size_t            offset                   = 0;
    };

    enum class generics_error {
        not_a_generic_type,
        target_type_does_not_exist,
        generic_count_mismatch,
        incomplete_type
    };

    struct type_information {
        std::vector<cstring>               generics;
        std::vector<type_field_definition> fields;
        cstring                            name             = cstring::empty();
        cstring                            module_path      = cstring::empty();
        primitive_types                    primitive_type   = primitive_types::unknown;
        reference_type                     reference_type   = reference_type::none;
        uint64_t                           fixed_array_size = 0;
        uint8_t                            alignment        = sizeof(uintptr_t);
        bool                               is_primitive     = false;
        bool                               is_const         = false;
        bool                               is_struct        = false;
        bool                               is_enum          = false;
        bool                               is_generic_type  = false;
        bool                               is_array         = false;
        bool                               is_dynamic_array = false;

        [[nodiscard]] size_t   get_size() const;
        [[nodiscard]] cstring  get_full_path() const;
        [[nodiscard]] uint64_t get_hash() const;

        void calculate_offsets();
        void add_field(const cstring& field_name, type_information* type);
        bool are_fields_valid() const;

        std::expected<type_information, generics_error> monomorphize(const std::vector<type_information*> types);
        static type_information                         get_for_primitive_type(primitive_types primitive_type);
    };
}


#endif //SHADE_TYPE_INFORMATION_H
