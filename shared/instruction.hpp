//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_INSTRUCTION_HPP
#define SHADE_INSTRUCTION_HPP

#include <cstddef>

#include "../runtime/vm/register.hpp"
#include "../vendor/sse2neon/sse2neon.h"


namespace shade {
    struct alignas(16) safe_reference_storage {
        std::byte * data;
        uint64_t size;
    };

    union store_64 {
        uint64_t u64;
        int64_t i64;
        uint32_t u32[2];
        int32_t i32[2];
        uint16_t u16[4];
        int16_t i16[4];
        uint8_t u8[8];
        int8_t i8[8];

        double imm_f64;
        float imm_f32[2];
    };

    struct alignas(16) vm_instruction {
        union {
            safe_reference_storage ref;
            std::byte * ptr[2];
            __m128 simd128;
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
        } data{};

        struct {
            union {
                std::byte * ptr;
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
            } imm;

            struct {
                uint8_t opcode;

                uint8_t reg_a;
                uint8_t reg_b;

                uint8_t a_type;
                uint8_t b_type;

                uint8_t result_type;

                union {
                    uint8_t raw;
                    uint8_t mode_reg   : 1;
                    uint8_t mode_stack : 1;
                    uint8_t mode_heap  : 1;
                    uint8_t mode_imm   : 1;
                    uint8_t unused     : 4;
                }  flags;

                uint8_t unused;

            };

        } instruction{};

        static_assert(sizeof(data) == 16 && sizeof(instruction) == 16);
        static_assert(sizeof(uintptr_t) == sizeof(uint64_t));
    };
}

#endif //SHADE_INSTRUCTION_HPP
