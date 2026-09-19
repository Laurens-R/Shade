//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#ifndef SHADE_TYPE_EXPRESSION_HPP
#define SHADE_TYPE_EXPRESSION_HPP
#include <cstdint>

namespace shade {
    struct type_definition;

    /**
     * A type_expression represents every type of type indicator in the language.
     * No matter if it's related to a funtion argument, a variable declaration, a struct field
     * or any other place where a type is defined on one shape or the other.
     */
    struct type_expression {
        type_definition *type = nullptr;
        size_t token_index_from;
        size_t token_index_to;
        uint64_t fixed_array_size = 0;
        bool is_owned = false;
        bool is_const = false;
        bool is_array = false;
        bool is_ptr = false;
        bool is_ref = false;
        bool is_dynamic_array = false;

        bool is_void() {
            return type == nullptr;
        }
    };
}

#endif //SHADE_TYPE_EXPRESSION_HPP
