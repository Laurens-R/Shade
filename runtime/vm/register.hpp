//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_REGISTER_HPP
#define SHADE_REGISTER_HPP
#include <cstdint>

#include "../../vendor/sse2neon/sse2neon.h"

namespace shade {

    inline constexpr const uint8_t REG_A = 0;
    inline constexpr const uint8_t REG_B = 0;
    inline constexpr const uint8_t REG_C = 0;
    inline constexpr const uint8_t REG_D = 0;
    inline constexpr const uint8_t REG_E = 0;
    inline constexpr const uint8_t REG_F = 0;
    inline constexpr const uint8_t REG_G = 0;
    inline constexpr const uint8_t REG_H = 0;

    struct vm_register {
        union {
            uint64_t u64[2];
            int64_t i64[2];
            uint32_t u32[4];
            int32_t i32[4];
            uint16_t u16[8];
            int16_t i16[8];
            uint8_t u8[16];
            int8_t i8[16];

            double imm_f64[2];
            float imm_f32[4];
        };
    };
}


#endif //SHADE_REGISTER_HPP
