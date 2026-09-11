//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_CTMAP_HPP
#define SHADE_CTMAP_HPP

#include <array>
#include <optional>
#include <stdexcept>
#include <string_view>

namespace shade
{
    template <typename TKey, typename TValue, std::size_t N>
    class const_map
    {
        static_assert(!(std::is_pointer_v<TKey> && std::is_same_v<std::remove_cv_t<std::remove_pointer_t<TKey>>, char>),
                      "Use std::string_view for string keys: const char* compares addresses, not text.");

    public:
        using key_type = TKey;
        using mapped_type = TValue;
        using value_type = std::pair<TKey, TValue>;

        constexpr const_map(const value_type (&items)[N]) : data_(std::to_array(items))
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                for (std::size_t j = i + 1; j < N; ++j)
                {
                    if (data_[i].first == data_[j].first)
                    {
                        throw std::logic_error("ConstMap: duplicate key");
                    }
                }
            }
        }

        // Returns a pointer to the value, or nullptr if the key is absent.
        constexpr const TValue * find(const TKey& key) const
        {
            for (const auto& entry : data_)
            {
                if (entry.first == key)
                {
                    return &entry.second;
                }
            }

            return nullptr;
        }

        constexpr bool contains(const TKey& key) const
        {
            return find(key) != nullptr;
        }

        // Throws at runtime if absent; fails to compile if used in a constant expression.
        constexpr const TValue& at(const TKey& key) const
        {
            if (const TValue* v = find(key))
            {
                return *v;
            }

            throw std::out_of_range("ConstMap: key not found");
        }

        static constexpr std::size_t size()
        {
            return N;
        }

        constexpr auto begin() const
        {
            return data_.begin();
        }

        constexpr auto end() const
        {
            return data_.end();
        }

    private:
        std::array<value_type, N> data_;
    };


    template <typename TKey, typename TValue, std::size_t N>
    constexpr const_map<TKey, TValue, N> make_map(const std::pair<TKey, TValue> (&items)[N])
    {
        return const_map<TKey, TValue, N>(items);
    }
}

#endif //SHADE_CTMAP_HPP
