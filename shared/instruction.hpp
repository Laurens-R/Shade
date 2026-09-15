//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_INSTRUCTION_HPP
#define SHADE_INSTRUCTION_HPP

#include "value.hpp"

namespace shade {
    struct alignas(16) VMInstruction {
        value_64     imm;
        value_64     imm2;
        std::uint8_t opcode;
        std::uint8_t r1;
        std::uint8_t r2;
        std::uint8_t r1type;
        std::uint8_t r2type;
        std::uint8_t resulttype;
        std::uint8_t flags1;
        std::uint8_t flags2;
    };
}

#endif //SHADE_INSTRUCTION_HPP
