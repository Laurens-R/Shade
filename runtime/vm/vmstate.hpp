//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_REGISTERS_HPP
#define SHADE_REGISTERS_HPP

#include "../../shared/vmvalue.hpp"

namespace shade {
    struct vmstate {
        uint64_t ip;
        uint64_t sp;
        reg r[8];
    };
}

#endif //SHADE_REGISTERS_HPP
