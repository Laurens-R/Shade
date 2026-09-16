#ifndef CLAW_FLAGUTILS_HPP
#define CLAW_FLAGUTILS_HPP

#include <cstdint>

namespace shade {
    /**
     * @brief Return @p state with @p flag set.
     * @param state The current bitset.
     * @param flag  The bit(s) to set.
     * @return The updated bitset.
     */
    constexpr auto set_flag(std::uint32_t state, std::uint32_t flag) -> std::uint32_t {
        return state | flag;
    }

    /**
     * @brief Return @p state with @p flag cleared.
     * @param state The current bitset.
     * @param flag  The bit(s) to clear.
     * @return The updated bitset.
     */
    constexpr auto clear_flag(std::uint32_t state, std::uint32_t flag) -> std::uint32_t {
        return state & ~flag;
    }

    /**
     * @brief Return @p state with @p flag toggled.
     * @param state The current bitset.
     * @param flag  The bit(s) to flip.
     * @return The updated bitset.
     */
    constexpr auto toggle_flag(std::uint32_t state, std::uint32_t flag) -> std::uint32_t {
        return state ^ flag;
    }

    /**
     * @brief Whether every bit in @p flag is set in @p state.
     * @param state The bitset to test.
     * @param flag  The bit(s) to check for.
     * @return True if set.
     */
    constexpr auto is_flag_set(std::uint32_t state, std::uint32_t flag) -> bool {
        return (state & flag) != 0;
    }
}

#endif //CLAW_FLAGUTILS_HPP
