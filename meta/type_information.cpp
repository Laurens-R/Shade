//
// Created by laure on 11/09/2026.
//

#include <optional>

#include "type_information.h"
#include "../utils/exceptions.hpp"
#include "../lang/spelling.hpp"


namespace shade {
    size_t type_information::get_size() const {
        if (is_primitive) {
            switch (primitive_type) {
                case primitive_types::i8:
                case primitive_types::u8:
                case primitive_types::boolean:
                    return sizeof(uint8_t);
                case primitive_types::i16:
                case primitive_types::u16:
                    return sizeof(uint16_t);
                case primitive_types::i32:
                case primitive_types::u32:
                case primitive_types::f32:
                    return sizeof(uint32_t);
                case primitive_types::i64:
                case primitive_types::u64:
                case primitive_types::f64:
                    return sizeof(uint64_t);
                case primitive_types::nothing:
                case primitive_types::unknown:
                default:
                    return 0;
            }
        }

        switch (reference_type) {
            case reference_type::pointer:
                return sizeof(uintptr_t);
            case reference_type::reference:
                return sizeof(uintptr_t);
            case reference_type::none:
            default:
                break;
        }

        switch (is_struct) {
                if (fields.empty()) {
                    return 0;
                }

                auto& last_field = fields.back();
                return last_field.offset + last_field.related_type_information->get_size();
        }

        if (is_enum) {
            return sizeof(int64_t);
        }

    }

    inline cstring type_information::get_full_path() const {
        cstring full_path = module_path + spelling::modules::module_seperator + name;



        return full_path;
    }

    uint64_t type_information::get_hash() const {
        const cstring path = get_full_path();

        if (path.is_empty()) {
            throw compiler_exception("get_hash: provided path was null or empty. A field must have a full path.");
        }

        return xxh::xxhash<64>(path.to_c_string(), path.length());
    }

    void type_information::calculate_offsets() {
        size_t current_offset = 0;

        for (auto& field : fields) {
            auto field_size       = field.related_type_information->get_size();
            auto current_boundary = current_offset % alignment;
            auto remaining_bytes  = alignment - current_boundary;

            if (field_size > alignment) {
                //field is bigger than the alignment
                field.offset   = current_offset;
                current_offset += remaining_bytes + field_size;
                continue;
            } else {
                //if we need to make it fit the natural alignment of the field
                size_t required_shift = remaining_bytes % field_size;
                current_offset        += required_shift;
                field.offset          = current_offset;
                current_offset        += field_size;
            }
        }
    }

    void type_information::add_field(const cstring& field_name, type_information* type) {
        if (type == nullptr) {
            throw compiler_exception("add_field: provided type was null.");
        }

        size_t current_offset   = fields.empty() ? 0 : fields.back().offset;
        auto   current_boundary = current_offset % alignment;
        auto   remaining_bytes  = alignment - current_boundary;
        size_t required_shift   = remaining_bytes % type->get_size();
        current_offset          += required_shift;

        fields.push_back({
            .name = name,
            .related_type_information = type,
            .offset = current_offset
        });
    }

    bool type_information::are_fields_valid() const {
        for (auto& field : fields) {
            if (field.related_type_information == nullptr) {
                return false;
            }
        }

        return true;
    }

    std::expected<type_information, generics_error> type_information::monomorphize(const std::vector<type_information*> types) {
        if (generics.empty()) {
            return std::unexpected(generics_error::not_a_generic_type);
        }

        auto generic_count       = generics.size();
        auto provided_type_count = types.size();

        if (generic_count != provided_type_count) {
            return std::unexpected(generics_error::generic_count_mismatch);
        }

        if (!are_fields_valid()) {
            return std::unexpected(generics_error::incomplete_type);
        }

        type_information new_type = *this;

        auto check_for_known_generics = [&](const cstring& name) -> std::optional<size_t> {
            for (size_t i = 0; i < generics.size(); i++) {
                auto & generic = generics.at(i);
                if (generic == name) {
                    return i;
                }
            }
            return std::nullopt;
        };

        for (auto& field : fields) {
            if (field.related_type_information->is_generic_type) {
                auto found_generic = check_for_known_generics(field.related_type_information->name);

                if (!found_generic.has_value()) {
                    continue;
                }

                field.related_type_information = types.at(found_generic.value());
                new_type.name += cstring("$") + field.related_type_information->name;
            }
        }

        calculate_offsets();

        return new_type;
    }

    type_information type_information::get_for_primitive_type(primitive_types primitive_type) {
        //TODO: implement all primitive types
        type_information result;
        result.is_primitive = true;

        switch (primitive_type) {
            case primitive_types::boolean:
                {
                    result.primitive_type = primitive_type;
                    result.name = spelling::types::boolean;
                }
            break;
        }

        return result;
    }


}
