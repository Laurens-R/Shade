//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#include "symbol_utils.hpp"
#include "../lang/spelling.hpp"

namespace shade {
    bool symbol_utils::is_valid_identifier(const cstring &symbol) {
        if (symbol.is_empty()) return false;

        if (symbol.is_bool()) return false;
        if (symbol.is_unsigned_int()) return false;
        if (symbol.is_signed_int()) return false;
        if (symbol.is_float()) return false;

        if (symbol == spelling::functions::keyword_func) return false;
        if (symbol == spelling::actions::keyword_return) return false;
        if (symbol == spelling::conditional::keyword_if) return false;
        if (symbol == spelling::conditional::keyword_else) return false;
        if (symbol == spelling::loops::keyword_for) return false;
        if (symbol == spelling::loops::keyword_while) return false;
        if (symbol == spelling::loops::keyword_foreach) return false;
        if (symbol == spelling::variables::keyword_auto_type) return false;
        if (symbol == spelling::variables::literal_null) return false;
        if (symbol == spelling::modules::keyword_module) return false;
        if (symbol == spelling::structs::keyword_struct) return false;
        if (symbol == spelling::types::keyword_constant) return false;
        if (symbol == spelling::types::keyword_owning) return false;
        if (symbol == spelling::types::keyword_reference) return false;
        if (symbol == spelling::types::keyword_pointer) return false;

        //native type names are also not allowed
        if (symbol == spelling::types::boolean) return false;
        if (symbol == spelling::types::float_32) return false;
        if (symbol == spelling::types::float_64) return false;
        if (symbol == spelling::types::keyword_string) return false;
        if (symbol == spelling::types::unsigned_int_8) return false;
        if (symbol == spelling::types::unsigned_int_16) return false;
        if (symbol == spelling::types::unsigned_int_32) return false;
        if (symbol == spelling::types::unsigned_int_64) return false;
        if (symbol == spelling::types::signed_int_8) return false;
        if (symbol == spelling::types::signed_int_16) return false;
        if (symbol == spelling::types::signed_int_32) return false;
        if (symbol == spelling::types::signed_int_64) return false;

        return true;
    }

    bool symbol_utils::is_integer_number(const cstring &symbol) {
        return symbol.is_unsigned_int() || symbol.is_signed_int() || symbol.is_hex_integer() || symbol.is_binary_integer();
    }

    bool symbol_utils::is_floating_point_number(const cstring &symbol) {
        return symbol.is_float();
    }

    bool symbol_utils::is_bool(const cstring &symbol) {
        return symbol.is_bool();
    }

    bool symbol_utils::is_primitive_literal(const cstring &symbol) {
        return is_integer_number(symbol) || is_floating_point_number(symbol) || is_bool(symbol);
    }

    cstring symbol_utils::get_approximate_literal_typename(const cstring &symbol) {
        if (symbol.is_unsigned_int()) return spelling::types::unsigned_int_64;
        if (symbol.is_signed_int()) return spelling::types::signed_int_64;
        if (symbol.is_hex_integer()) return spelling::types::unsigned_int_64;
        if (symbol.is_binary_integer()) return spelling::types::unsigned_int_64;
        if (symbol.is_bool()) return spelling::types::boolean;
        if (symbol.is_float()) return spelling::types::float_64;
    }

    bool symbol_utils::is_operator(const cstring &symbol) {
        /*if (node->is_unary_operation) return 12;
        if (node->type == ast_node_types::assignment) return 1;
        if (node->type == ast_node_types::return_statement) return 1;
        if (node->type == ast_node_types::conditional) return 2;*/
        return symbol == spelling::logic::logic_or
               || symbol == spelling::logic::logic_and
               || symbol == spelling::operators::operator_bitwise_or
               || symbol == spelling::operators::operator_bitwise_xor
               || symbol == spelling::operators::operator_bitwise_and
               || symbol == spelling::comparisons::comparison_equal
               || symbol == spelling::comparisons::comparison_not_equal
               || symbol == spelling::comparisons::comparison_less
               || symbol == spelling::comparisons::comparison_greater
               || symbol == spelling::comparisons::comparison_less_equal
               || symbol == spelling::comparisons::comparison_greater_equal
               || symbol == spelling::operators::operator_lshift
               || symbol == spelling::operators::operator_rshift
               || symbol == spelling::operators::operator_add
               || symbol == spelling::operators::operator_sub
               || symbol == spelling::operators::operator_mul
               || symbol == spelling::operators::operator_div
               || symbol == spelling::operators::operator_mod;
    }
} // shade
