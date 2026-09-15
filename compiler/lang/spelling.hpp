//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_KNOWNTOKENS_HPP
#define SHADE_KNOWNTOKENS_HPP

namespace shade {

    namespace spelling {
        namespace compiler {
            inline constexpr const char* source_ext = ".shade";
        }

        namespace actions {
            inline constexpr const char* end_of_statement = ";";
            inline constexpr const char* return_action    = "return";
        }

        namespace variables {
            inline constexpr const char* assignment   = "=";
            inline constexpr const char* null_literal = "null";
            inline constexpr const char* auto_type    = "var";
        }

        namespace scopes {
            inline constexpr const char* begin_body = "{";
            inline constexpr const char* end_body   = "}";
        }

        namespace groups {
            inline constexpr const char* begin_group                = "(";
            inline constexpr const char* end_group                  = ")";
            inline constexpr const char* group_seperator_arguments  = ",";
            inline constexpr const char* group_seperator_directives = ";";
        }

        namespace attributes {
            inline constexpr const char* attr_indicator          = "@";
            inline constexpr const char* attr_begin_arguments    = groups::begin_group;
            inline constexpr const char* attr_end_arguments      = groups::end_group;
            inline constexpr const char* attr_argument_seperator = groups::group_seperator_arguments;
        }

        namespace modules {
            inline constexpr const char* module_name      = "module";
            inline constexpr const char* module_seperator = "::";
            inline constexpr const char* begin_body       = scopes::begin_body;
            inline constexpr const char* end_body         = scopes::end_body;
        }

        namespace structs {
            inline constexpr const char* struct_indicator = "struct";
            inline constexpr const char* begin_body       = scopes::begin_body;
            inline constexpr const char* end_body         = scopes::end_body;
        }

        namespace functions {
            inline constexpr const char* func_indicator          = "func";
            inline constexpr const char* func_return_statement   = "return";
            inline constexpr const char* func_return_type        = "->";
            inline constexpr const char* begin_arguments         = groups::begin_group;
            inline constexpr const char* end_arguments           = groups::end_group;
            inline constexpr const char* func_argument_seperator = groups::group_seperator_arguments;
            inline constexpr const char* lambda_begin_capture    = "[";
            inline constexpr const char* labda_end_capture       = "]";
            inline constexpr const char* begin_body              = scopes::begin_body;
            inline constexpr const char* end_body                = scopes::end_body;
        }

        namespace generics {
            inline constexpr const char* generic_begin = "<";
            inline constexpr const char* generic_end   = ">";
        }


        namespace conditional {
            inline constexpr const char* if_start                  = "if";
            inline constexpr const char* if_else                   = "else";
            inline constexpr const char* begin_condition_group     = groups::begin_group;
            inline constexpr const char* end_condition_group       = groups::end_group;
            inline constexpr const char* begin_body                = scopes::begin_body;
            inline constexpr const char* end_body                  = scopes::end_body;
            inline constexpr const char* inline_conditional_symbol = "?";
            inline constexpr const char* inline_if_else_symbol     = ":";
        }

        namespace loops {
            inline constexpr const char* loop_for            = "for";
            inline constexpr const char* loop_foreach        = "foreach";
            inline constexpr const char* loop_while          = "while";
            inline constexpr const char* begin_conditions    = groups::begin_group;
            inline constexpr const char* end_conditions      = groups::end_group;
            inline constexpr const char* condition_seperator = groups::group_seperator_directives;
            inline constexpr const char* begin_body          = scopes::begin_body;
            inline constexpr const char* end_body            = scopes::end_body;
        }

        namespace operators {
            inline constexpr const char* operator_add                = "+";
            inline constexpr const char* operator_sub                = "-";
            inline constexpr const char* operator_mul                = "*";
            inline constexpr const char* operator_div                = "/";
            inline constexpr const char* operator_mod                = "%";
            inline constexpr const char* operator_lshift             = "<<";
            inline constexpr const char* operator_rshift             = ">>";
            inline constexpr const char* operator_bitwise_and        = "&";
            inline constexpr const char* operator_bitwise_or         = "|";
            inline constexpr const char* operator_bitwise_xor        = "^";
            inline constexpr const char* operator_bitwise_or_assign  = "|=";
            inline constexpr const char* operator_bitwise_xor_assign = "^=";
            inline constexpr const char* operator_bitwise_and_assign = "&=";
        }

        namespace comparisons {
            inline constexpr const char* comparison_equal         = "==";
            inline constexpr const char* comparison_not_equal     = "!=";
            inline constexpr const char* comparison_greater       = ">";
            inline constexpr const char* comparison_less          = "<";
            inline constexpr const char* comparison_greater_equal = ">=";
            inline constexpr const char* comparison_less_equal    = "<=";
        }

        namespace logic {
            inline constexpr const char* logic_not = "!";
            inline constexpr const char* logic_and = "&&";
            inline constexpr const char* logic_or  = "||";
        }

        namespace literals {
            inline constexpr const char* literal_string_indicator = "\"";
            inline constexpr const char* litaral_negative_number  = operators::operator_sub;
            inline constexpr const char* literal_decimal_symbol   = ".";
        }

        namespace types {
            inline constexpr const char* owning            = "owned";
            inline constexpr const char* constant          = "const";
            inline constexpr const char* reference         = "ref";
            inline constexpr const char* pointer           = "ptr";
            inline constexpr const char* string            = "string";
            inline constexpr const char* boolean           = "bool";
            inline constexpr const char* unsigned_int_8    = "u8";
            inline constexpr const char* unsigned_int_16   = "u16";
            inline constexpr const char* unsigned_int_32   = "u32";
            inline constexpr const char* unsigned_int_64   = "u64";
            inline constexpr const char* signed_int_8      = "i8";
            inline constexpr const char* signed_int_16     = "i16";
            inline constexpr const char* signed_int_32     = "i32";
            inline constexpr const char* signed_int_64     = "i64";
            inline constexpr const char* float_32          = "f32";
            inline constexpr const char* float_64          = "f64";
            inline constexpr const char* array_index_begin = "[";
            inline constexpr const char* array_index_end   = "]";
            inline constexpr const char* dynamic_array     = "[]";
        }
    }
}

#endif //SHADE_KNOWNTOKENS_HPP
