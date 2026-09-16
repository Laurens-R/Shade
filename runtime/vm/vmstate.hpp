//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_REGISTERS_HPP
#define SHADE_REGISTERS_HPP

#include "register.hpp"
#include "../vendor/sse2neon/sse2neon.h"

namespace shade {
    struct vmstate {
        uint64_t ip;
        uint64_t sp;
        uint64_t ret;
        uint64_t stack_frame;
        vm_register regs[8];
        alignas(16) __m128 simd128[4];
    };
}

#endif //SHADE_REGISTERS_HPP
