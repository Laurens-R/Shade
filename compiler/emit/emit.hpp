//
// Created by Laurens Ruijtenberg on 17/09/2026.
//

#ifndef SHADE_EMIT_HPP
#define SHADE_EMIT_HPP

#include <functional>
#include <utility>
#include <vector>
#include <shared/instruction.hpp>
#include <shared/opcodes.hpp>
#include <shared/vmconstants.hpp>

namespace shade {
    class emit {
        private:
            std::vector<vm_instruction> instructions;

        public:
            void mov(reg_id reg_a, reg_id reg_b) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::mov);
                i.instruction.reg_a = reg_a;
                i.instruction.reg_b = reg_b;
                i.instruction.flags.mode_reg = true;
                instructions.push_back(i);
            }

            void mov(reg_id reg_a, value_64 imm) {
                vm_instruction i{};
                i.instruction.imm = imm;
                i.instruction.opcode = std::to_underlying(opcodes::mov);
                i.instruction.reg_a = reg_a;
                i.instruction.flags.mode_imm = true;
                instructions.push_back(i);
            }

            void load(reg_id reg_a, value_64 imm) {
                vm_instruction i{};
                i.instruction.imm = imm;
                i.instruction.opcode = std::to_underlying(opcodes::load);
                i.instruction.reg_a = reg_a;
                i.instruction.flags.mode_heap = true;
                instructions.push_back(i);
            }

            void store(reg_id reg_a, value_64 imm) {
                vm_instruction i{};
                i.instruction.imm = imm;
                i.instruction.opcode = std::to_underlying(opcodes::store);
                i.instruction.reg_a = reg_a;
                i.instruction.flags.mode_heap = true;
                instructions.push_back(i);
            }

            void stack_load(reg_id reg_a, stack_offset stack_offset) {
                vm_instruction i{};
                i.instruction.imm.u64 = stack_offset;
                i.instruction.opcode = std::to_underlying(opcodes::sload);
                i.instruction.reg_a = reg_a;
                i.instruction.flags.mode_stack = true;
                instructions.push_back(i);
            }

            void stack_store(reg_id reg_a, stack_offset stack_offset) {
                vm_instruction i{};
                i.instruction.imm.u64 = stack_offset;
                i.instruction.opcode = std::to_underlying(opcodes::sstore);
                i.instruction.reg_a = reg_a;
                i.instruction.flags.mode_stack = true;
                instructions.push_back(i);
            }

            void add(reg_id reg_a, reg_id reg_b) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::add);
                i.instruction.reg_a = reg_a;
                i.instruction.reg_b = reg_b;
                i.instruction.flags.mode_reg = true;
                instructions.push_back(i);
            }

            void sub(reg_id reg_a, reg_id reg_b) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::sub);
                i.instruction.reg_a = reg_a;
                i.instruction.reg_b = reg_b;
                i.instruction.flags.mode_reg = true;
                instructions.push_back(i);
            }

            void div(reg_id reg_a, reg_id reg_b) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::div);
                i.instruction.reg_a = reg_a;
                i.instruction.reg_b = reg_b;
                i.instruction.flags.mode_reg = true;
                instructions.push_back(i);
            }

            void mul(reg_id reg_a, reg_id reg_b) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::mul);
                i.instruction.reg_a = reg_a;
                i.instruction.reg_b = reg_b;
                i.instruction.flags.mode_reg = true;
                instructions.push_back(i);
            }

            void jump(uint64_t jump_to_offset) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::jmp);
                i.instruction.imm.u64 = jump_to_offset;
                instructions.push_back(i);
            }

            void jump_not_zero(uint64_t jump_to_offset, reg_id reg_to_check) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::jmp_nz);
                i.instruction.imm.u64 = jump_to_offset;
                i.instruction.reg_a = reg_to_check;
                instructions.push_back(i);
            }

            void call(uint64_t call_to_offset) {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::call);
                i.instruction.imm.u64 = call_to_offset;
                instructions.push_back(i);
            }

            void ret() {
                vm_instruction i{};
                i.instruction.opcode = std::to_underlying(opcodes::ret);
                instructions.push_back(i);
            }
    };
} // shade

#endif //SHADE_EMIT_HPP
