//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_REGISTER_HPP
#define SHADE_REGISTER_HPP
#include <cstdint>

#include "../vendor/xxhash/sse2neon.h"

namespace shade {

    struct value_64 {
        union {
            uint64_t u64;
            int64_t i64;
            uint32_t u32[2];
            int32_t i32[2];
            uint16_t u16[4];
            int16_t i16[4];
            uint8_t u8[8];
            int8_t i8[8];

            double f64;
            float f32[2];
        };
    };

    using reg = value_64;
}


#endif //SHADE_REGISTER_HPP
