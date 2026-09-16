//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_OPERATIONS_HPP
#define SHADE_OPERATIONS_HPP

#include "../lexer/lexer.hpp"
#include "../../shared/utils/ctmap.hpp"
#include "spelling.hpp"

namespace shade
{
    enum class ops_and_actions
    {
        assignment,

        //math
        add,
        substract,
        multiply,
        divide,
        modulo,

        //logic
        logical_and,
        logical_or,
        logical_not,

        //bitwise logic
        bitwise_and,
        bitwise_or,
        bitwise_xor,
        bitwise_not,
        bitwise_or_assign,
        bitwise_xor_assign,
        bitwise_and_assign,

        //bit operations
        bitwise_left_shift,
        bitwise_right_shift,

        //comparison,
        compare_equal,
        compare_not_equal,
        compare_greater,
        compare_less,
        compare_greater_equal,
        compare_less_equal,

        define_variable,
        define_struct,
        call_function,
        call_function_ptr,
        call_c_function_ptr,
        return_value,
        start_if_block,
        start_else_block,
        start_for_loop,
        start_while_loop,
        start_foreach_loop,

        unknown
    };

    inline ops_and_actions get_operation_from_token(const language_token& token)
    {
        static constexpr auto action_map = make_map<std::string_view, ops_and_actions>({
            {spelling::variables::assignment, ops_and_actions::assignment},

            //math
            {spelling::operators::operator_add, ops_and_actions::add},
            {spelling::operators::operator_sub, ops_and_actions::substract},
            {spelling::operators::operator_mul, ops_and_actions::multiply},
            {spelling::operators::operator_div, ops_and_actions::divide},
            {spelling::operators::operator_mod, ops_and_actions::modulo},

            //logic
            {spelling::logic::logic_and, ops_and_actions::logical_and},
            {spelling::logic::logic_or, ops_and_actions::logical_or},
            {spelling::logic::logic_not, ops_and_actions::logical_not},

            //bitwise
            {spelling::operators::operator_bitwise_and, ops_and_actions::bitwise_and},
            {spelling::operators::operator_bitwise_or, ops_and_actions::bitwise_or},
            {spelling::operators::operator_bitwise_xor, ops_and_actions::bitwise_xor},
            {spelling::operators::operator_bitwise_or_assign, ops_and_actions::bitwise_or_assign},
            {spelling::operators::operator_bitwise_xor_assign, ops_and_actions::bitwise_xor_assign},
            {spelling::operators::operator_bitwise_and_assign, ops_and_actions::bitwise_and_assign},
            {spelling::operators::operator_lshift, ops_and_actions::bitwise_left_shift},
            {spelling::operators::operator_rshift, ops_and_actions::bitwise_right_shift},

            //comparison
            {spelling::comparisons::comparison_equal, ops_and_actions::compare_equal},
            {spelling::comparisons::comparison_not_equal, ops_and_actions::compare_not_equal},
            {spelling::comparisons::comparison_greater, ops_and_actions::compare_greater},
            {spelling::comparisons::comparison_less, ops_and_actions::compare_less},
            {spelling::comparisons::comparison_greater_equal, ops_and_actions::compare_greater_equal},
            {spelling::comparisons::comparison_less_equal, ops_and_actions::compare_less_equal},

            //actions
            {spelling::actions::return_action, ops_and_actions::return_value},
            {spelling::conditional::if_start, ops_and_actions::start_if_block},
            {spelling::conditional::if_else, ops_and_actions::start_else_block},
            {spelling::loops::loop_for, ops_and_actions::start_for_loop},
            {spelling::loops::loop_foreach, ops_and_actions::start_foreach_loop},
            {spelling::loops::loop_while, ops_and_actions::start_while_loop}
        });

        if (const auto result = action_map.find(token.text.to_std_string_view()); result != nullptr)
        {
            return *result;
        }

        return ops_and_actions::unknown;

    }
}

#endif //SHADE_OPERATIONS_HPP
