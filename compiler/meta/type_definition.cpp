//
// Created by laure on 11/09/2026.
//

#include <optional>

#include "type_definition.hpp"

#include <unordered_map>

#include "../utils/exceptions.hpp"
#include "../utils/ctmap.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    size_t type_definition::get_size() const {
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

        if (is_struct) {
            if (fields.empty()) {
                return 0;
            }

            auto& last_field = fields.back();
            return last_field.offset + last_field.related_type_information->get_size();
        }

        if (is_enum) {
            return sizeof(int64_t);
        }

        return 0;
    }

    uint64_t type_definition::get_hash() const {
        const cstring path = full_path;

        if (path.is_empty()) {
            throw compiler_exception("get_hash: provided path was null or empty. A field must have a full path.");
        }

        return xxh::xxhash<64>(path.to_c_string(), path.length());
    }

    void type_definition::calculate_offsets() {
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

    void type_definition::add_field(const cstring& field_name, type_definition* type) {
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

    function_definition *type_definition::add_method(const function_definition &func_def) {
        methods.push_back(func_def);
        auto added_method = &methods.back();
        added_method->related_type = this;
        added_method->full_path = full_path + spelling::modules::module_seperator + added_method->name;
        added_method->related_module = related_module;
        added_method->module_path = related_module->full_path;
        return added_method;
    }

    bool type_definition::are_fields_valid() const {
        for (auto& field : fields) {
            if (field.related_type_information == nullptr) {
                return false;
            }
        }

        return true;
    }

    std::expected<type_definition, generics_error> type_definition::monomorphize(const std::vector<type_definition*> types) {
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

        type_definition new_type = *this;

        auto check_for_known_generics = [&](const cstring& name) -> std::optional<size_t> {
            for (size_t i = 0; i < generics.size(); i++) {
                auto& generic = generics.at(i);
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
                new_type.name                  += cstring("$") + field.related_type_information->name;
            }
        }

        calculate_offsets();

        return new_type;
    }

    type_definition type_definition::get_for_primitive_type(primitive_types primitive_type) {
        if (primitive_type == primitive_types::nothing || primitive_type == primitive_types::unknown) {
            throw compiler_exception("Tried to get primitive type for nothing or unknown.");
        }

        static constexpr auto typemap = make_map<primitive_types, std::string_view>({
            {primitive_types::u8, spelling::types::unsigned_int_8},
            {primitive_types::u16, spelling::types::unsigned_int_16},
            {primitive_types::u32, spelling::types::unsigned_int_32},
            {primitive_types::u64, spelling::types::unsigned_int_64},
            {primitive_types::i8, spelling::types::signed_int_8},
            {primitive_types::i16, spelling::types::signed_int_16},
            {primitive_types::i32, spelling::types::signed_int_32},
            {primitive_types::i64, spelling::types::signed_int_64},
            {primitive_types::f32, spelling::types::float_32},
            {primitive_types::f64, spelling::types::float_64},
            {primitive_types::string, spelling::types::string},
            {primitive_types::boolean, spelling::types::boolean}
        });

        type_definition result = {};
        result.is_primitive     = true;
        result.primitive_type   = primitive_type;
        result.name             = typemap.at(primitive_type);
        result.module_path      = path_utils::get_global_path() + cstring::empty();
        result.full_path        = result.name;

        return result;
    }

    type_definition type_definition::create_struct(const cstring& full_path, ast_node* related_node, size_t alignment) {
        type_definition result;

        result.name         = path_utils::get_object_from_path(full_path);
        result.full_path    = full_path;
        result.module_path  = path_utils::get_namespace_from_path(full_path);
        result.is_struct    = true;
        result.alignment    = alignment;
        result.related_node = related_node;

        return result;
    }

    metadata_type type_definition::get_type() {
        return metadata_type::type;
    }
}
