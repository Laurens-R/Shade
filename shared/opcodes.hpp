//
// Created by Laurens Ruijtenberg on 16/09/2026.
//

#ifndef SHADE_OPCODES_HPP
#define SHADE_OPCODES_HPP
#include <cstdint>

#include "instruction.hpp"

namespace shade {


    enum class opcodes : uint8_t {
        //no operation
        nop,

        //push the value from the provided register or memory address on the stack
        push,

        //push n new items on the stack (un-initialized)
        push_n,

        //pop from the stack
        pop,

        //pop n items from the stack
        pop_n,

        //peek at the value on the stack and store it in the provided register or memory address
        peek,

        load,

        store,

        sload,

        sstore,

        //move an immediate value onto the provided register
        mov,

        //add two values from two registers
        add,

        //substract two values from two registers
        sub,

        //divide two values from two registers
        div,

        //multiply tow values from two registers
        mul,

        //modulo two values from two registers
        mod,

        //left shift
        lshift,

        //right shift
        rshift,

        //logical and
        logical_and,

        //logical or
        logical_or,

        //logical not
        logical_not,

        //bitwise and
        bitwise_and,

        //bitwise or
        bitwise_or,

        //bitwise not
        bitwise_not,

        //bitwise xor
        bitwise_xor,

        //compare if values are equal
        cmp_eq,

        //compare if values are not equal
        cmp_neq,

        //call a function
        call,

        //return from a function to the previous stackframe
        ret,

        //call a native (host) function
        call_host,

        //jmp to a instruction at the specified position
        jmp,

        //jmp to a instruction at the specified position if the value in REG_A is not zero
        jmp_nz
    };
}
#endif //SHADE_OPCODES_HPP
