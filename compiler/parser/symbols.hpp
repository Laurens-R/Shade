//
// Created by Laurens Ruijtenberg on 18/09/2026.
//

#ifndef SHADE_SYMBOLS_HPP
#define SHADE_SYMBOLS_HPP
#include "shared/utils/cstring.hpp"

namespace shade {
    class symbols {
        static bool is_valid_identifier(const cstring &identifier);
    };
} // shade

#endif //SHADE_SYMBOLS_HPP
