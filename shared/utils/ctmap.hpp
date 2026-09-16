#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace shade {

    template <typename TKey, typename TValue, std::size_t Capacity = 256>
    class dense_enum_map {
        static_assert(std::is_enum_v<TKey> || std::is_integral_v<TKey>,
                      "dense_enum_map requires an enum or integral key type.");

    public:
        using key_type   = TKey;
        using value_type = TValue;

        template <std::size_t N>
        constexpr dense_enum_map(const std::pair<TKey, TValue> (&items)[N], TValue default_value = TValue{})
            : table_{} 
        {
            table_.fill(default_value);

            // O(N) mapping and duplicate/bounds checking happens ENTIRELY at compile time:
            for (std::size_t i = 0; i < N; ++i) {
                const auto idx = static_cast<std::size_t>(items[i].first);
                if (idx >= Capacity) {
                    throw std::out_of_range("dense_enum_map: key exceeds capacity");
                }
                table_[idx] = items[i].second;
            }
        }

        // Direct O(1) runtime lookup - single indexed memory load (mov rax, [table + rdx*8])
        [[nodiscard]] constexpr const TValue& operator[](TKey key) const noexcept {
            return table_[static_cast<std::size_t>(key)];
        }

        [[nodiscard]] constexpr const TValue& operator[](std::size_t index) const noexcept {
            return table_[index];
        }

        [[nodiscard]] constexpr std::size_t size() const noexcept {
            return Capacity;
        }

        [[nodiscard]] constexpr auto begin() const noexcept { return table_.begin(); }
        [[nodiscard]] constexpr auto end() const noexcept { return table_.end(); }

    private:
        std::array<TValue, Capacity> table_;
    };

    // Helper factory function:
    template <typename TKey, typename TValue, std::size_t Capacity = 256, std::size_t N>
    constexpr dense_enum_map<TKey, TValue, Capacity> make_map(
        const std::pair<TKey, TValue> (&items)[N], 
        TValue default_value = TValue{}
    ) {
        return dense_enum_map<TKey, TValue, Capacity>(items, default_value);
    }

}