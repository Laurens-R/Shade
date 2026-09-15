//
// Created by Laurens Ruijtenberg on 15/09/2026.
//

#ifndef SHADE_CAPTUREMETHODS_HPP
#define SHADE_CAPTUREMETHODS_HPP

#include "../lang/spelling.hpp"
#include "range_capture.hpp"
#include "parser_range_keys.hpp"

namespace shade {
    struct capture_methods {
        range_capture attribute_range;
        range_capture module_range;
        range_capture struct_range;
        range_capture function_range;
        range_capture if_range;
        range_capture if_else_range;
        range_capture else_range;
        range_capture for_loop_range;
        range_capture while_loop_range;
        range_capture raw_scope_range;
        range_capture statement_range;

        capture_methods();

        void init();
    };
} // shade

#endif //SHADE_CAPTUREMETHODS_HPP
