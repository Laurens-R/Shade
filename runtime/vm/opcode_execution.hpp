//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_OPCODE_EXECUTION_HPP
#define SHADE_OPCODE_EXECUTION_HPP

#include <shared/instruction.hpp>
#include <shared/opcodes.hpp>
#include <shared/utils/ctmap.hpp>

namespace shade {
    using opcode_function = void (*)(const vm_instruction &instruction) noexcept;

    struct opcode_nop {
        static void execute(const vm_instruction &instruction) noexcept {}
    };

    inline constexpr auto opcode_functions = make_map<opcodes, opcode_function>({
        {opcodes::nop, &opcode_nop::execute}
    });


}

#endif //SHADE_OPCODE_EXECUTION_HPP
