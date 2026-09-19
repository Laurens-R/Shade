//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#ifndef SHADE_SYMBOLS_HPP
#define SHADE_SYMBOLS_HPP
#include "shared/utils/cstring.hpp"

namespace shade {
    class symbol_utils {
        public:
            static bool is_valid_identifier(const cstring &symbol);

            static bool is_integer_number(const cstring &symbol);

            static bool is_floating_point_number(const cstring &symbol);

            static bool is_bool(const cstring &symbol);

            static bool is_primitive_literal(const cstring &symbol);

            static cstring get_approximate_literal_typename(const cstring &symbol);

            static bool is_operator(const cstring &symbol);
    };
} // shade

#endif //SHADE_SYMBOLS_HPP
