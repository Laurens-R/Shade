//
// Created by laure on 10/09/2026.
//

#ifndef SHADE_KNOWNTOKENS_HPP
#define SHADE_KNOWNTOKENS_HPP

namespace shade {

    namespace spelling {
        namespace compiler {
            inline const char* source_ext = ".shade";
        }

        namespace scopes {
            inline const char* begin_scope = "{";
            inline const char* end_scope = "}";
        }

        namespace groups {
            inline const char* begin_group = "(";
            inline const char* end_group = ")";
            inline const char* group_seperator_arguments = ",";
            inline const char* group_seperator_directives = ";";
        }

        namespace attributes {
            inline const char* attr_inline = "@inline";
            inline const char* attr_async = "@async";
            inline const char* attr_alignment = "@alignas";
            inline const char* attr_begin_arguments = groups::begin_group;
            inline const char* attr_end_arguments = groups::end_group;
            inline const char* attr_argument_seperator = groups::group_seperator_arguments;
        }

        namespace modules {
            inline const char* module_name = "module";
            inline const char* module_seperator = "::";
            inline const char* begin_scope = groups::begin_group;
            inline const char* end_scope = groups::end_group;
        }

        namespace structs {
            inline const char* struct_indicator = "struct";
            inline const char* struct_begin_scope = scopes::begin_scope;
            inline const char* struct_end_scope = scopes::end_scope;
        }

        namespace functions {
            inline const char* func_indicator = "func";
            inline const char* func_return_statement = "return";
            inline const char* func_return_type = "->";
            inline const char* func_start_arguments = groups::begin_group;
            inline const char* func_end_arguments = groups::end_group;
            inline const char* func_argument_seperator = groups::group_seperator_arguments;
            inline const char* lambda_begin_capture = "[";
            inline const char* labda_end_capture = "]";
            inline const char* func_begin_scope = scopes::begin_scope;
            inline const char* func_end_scope = scopes::end_scope;
        }

        namespace generics {
            inline const char* generic_begin = "<";
            inline const char* generic_end = ">";
        }


        namespace conditional {
            inline const char* if_start = "if";
            inline const char* if_else   = "else";
            inline const char* begin_condition_group = groups::begin_group;
            inline const char* end_condition_group = groups::end_group;
            inline const char* begin_scope = scopes::begin_scope;
            inline const char* end_scope = scopes::end_scope;
            inline const char* inline_conditional_symbol = "?";
            inline const char* inline_if_else_symbol = ":";
        }

        namespace loops {
            inline const char* loop_for = "for";
            inline const char* loop_foreach = "foreach";
            inline const char* loop_while = "while";
            inline const char* start_conditions = groups::begin_group;
            inline const char* end_conditions = groups::end_group;
            inline const char* condition_seperator = groups::group_seperator_directives;
            inline const char* begin_scope = scopes::begin_scope;
            inline const char* end_scope = scopes::end_scope;
        }

        namespace operators {
            inline const char* operator_add = "+";
            inline const char* operator_sub = "-";
            inline const char* operator_mul = "*";
            inline const char* operator_div = "/";
            inline const char* operator_mod = "%";
            inline const char* operator_lshift = "<<";
            inline const char* operator_rshift = ">>";
            inline const char* operator_bitwise_and = "&";
            inline const char* operator_bitwise_or = "|";
            inline const char* operator_bitwise_xor = "^";
            inline const char* operator_bitwise_or_assign = "|=";
            inline const char* operator_bitwise_xor_assign = "^=";
            inline const char* operator_bitwise_and_assign = "&=";
        }

        namespace comparisons {
            inline const char* comparison = "==";
            inline const char* comparison_equal = "!=";
            inline const char* comparison_greater = ">";
            inline const char* comparison_less = "<";
            inline const char* comparison_greater_equal = ">=";
            inline const char* comparison_less_equal = "<=";
        }

        namespace logic {
            inline const char* logic_not = "!";
            inline const char* logic_and = "&&";
            inline const char* logic_or = "||";
        }

        namespace literals {
            inline const char* literal_string_indicator = "\"";
            inline const char* litaral_negative_number = operators::operator_sub;
            inline const char* literal_decimal_symbol = ".";
        }

        namespace types {
            inline const char* owning = "owned";
            inline const char* constant = "const";
            inline const char* reference = "ref";
            inline const char* pointer = "ptr";
            inline const char* string = "string";
            inline const char* boolean = "bool";
            inline const char* unsigned_int_8  = "u8";
            inline const char* unsigned_int_16 = "u16";
            inline const char* unsigned_int_32 = "u32";
            inline const char* unsigned_int_64 = "u64";
            inline const char* signed_int_8    = "i8";
            inline const char* signed_int_16   = "i16";
            inline const char* signed_int_32   = "i32";
            inline const char* signed_int_64   = "i64";
            inline const char* array_index_begin = "[";
            inline const char* array_index_end = "]";
        }
    }
}

#endif //SHADE_KNOWNTOKENS_HPP
