//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "symbols.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    bool symbols::is_valid_identifier(const cstring &identifier) {
        if (identifier.is_empty()) return false;

        if (identifier.is_bool()) return false;
        if (identifier.is_unsigned_int()) return false;
        if (identifier.is_signed_int()) return false;
        if (identifier.is_float()) return false;

        if (identifier == spelling::functions::keyword_func) return false;
        if (identifier == spelling::actions::keyword_return) return false;
        if (identifier == spelling::conditional::keyword_if) return false;
        if (identifier == spelling::conditional::keyword_else) return false;
        if (identifier == spelling::loops::keyword_for) return false;
        if (identifier == spelling::loops::keyword_while) return false;
        if (identifier == spelling::loops::keyword_foreach) return false;
        if (identifier == spelling::variables::keyword_auto_type) return false;
        if (identifier == spelling::variables::literal_null) return false;
        if (identifier == spelling::modules::keyword_module) return false;
        if (identifier == spelling::structs::keyword_struct) return false;
        if (identifier == spelling::types::keyword_constant) return false;
        if (identifier == spelling::types::keyword_owning) return false;
        if (identifier == spelling::types::keyword_reference) return false;
        if (identifier == spelling::types::keyword_pointer) return false;

        //native type names are also not allowed
        if (identifier == spelling::types::boolean) return false;
        if (identifier == spelling::types::float_32) return false;
        if (identifier == spelling::types::float_64) return false;
        if (identifier == spelling::types::keyword_string) return false;
        if (identifier == spelling::types::unsigned_int_8) return false;
        if (identifier == spelling::types::unsigned_int_16) return false;
        if (identifier == spelling::types::unsigned_int_32) return false;
        if (identifier == spelling::types::unsigned_int_64) return false;
        if (identifier == spelling::types::signed_int_8) return false;
        if (identifier == spelling::types::signed_int_16) return false;
        if (identifier == spelling::types::signed_int_32) return false;
        if (identifier == spelling::types::signed_int_64) return false;

        return true;
    }
} // shade