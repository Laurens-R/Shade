#ifndef CLAW_NODESTRING_HPP
#define CLAW_NODESTRING_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace shade {
    /**
     * @brief An owning, growable C-string with parsing/conversion helpers.
     *
     * Backed by a NUL-terminated heap buffer that doubles on append. Also provides the
     * literal recognition (`IsI32`, `IsFloat`, …) and conversion (`ToI32`, `ToFloat64`, …)
     * the lexer and type system use to classify and decode source literals.
     */
    class cstring {
    private:
        size_t                  _capacity      = 0;
        size_t                  _currentOffset = 0;
        std::unique_ptr<char[]> _str;

        /// Grow (or allocate) the buffer to hold @p length chars plus a NUL, preserving content.
        auto alloc(size_t capacity, bool cleanMemory = true) -> void;

    public:
        /// @brief Construct an empty string with default capacity.
        explicit cstring();

        /// @brief Construct a one-character string.
        explicit cstring(const char c);

        /// @brief Construct from a std::string.
        explicit cstring(const std::string& initialValue);

        /// @brief Construct an empty string with at least @p initialLength capacity.
        explicit cstring(const size_t initialLength);

        /// @brief Construct from a raw byte range (need not be NUL-terminated): copies @p length bytes
        /// and terminates. Used to materialize an arena-view runtime string (see VMSharedState).
        cstring(const char* data, size_t length);

        /// @brief Construct from a NUL-terminated C string (implicit on purpose).
        cstring(const char* initialValue);

        cstring(const cstring& other);

        cstring(const cstring& other, const size_t bufferReservation);

        /// @brief Move — steals the backing buffer (no allocation). `noexcept` so std::vector relocates
        /// (rather than deep-copies) the string pools on growth, and so `std::move(str)` into the runtime
        /// string pool is a pointer steal, not a heap copy. Declaring copy ops suppresses the implicit
        /// move, so these MUST be declared explicitly or every `std::move` silently deep-copies.
        cstring(cstring&& other) noexcept;

        auto operator=(cstring&& other) noexcept -> cstring&;

        auto operator=(const cstring& other) -> cstring&;

        auto operator=(const char* other) -> cstring&;

        auto operator=(const std::string_view & other) -> cstring&;

        /// @brief Concatenate, returning a new string.
        auto operator+(const cstring& other) const -> cstring;

        /// @brief Concatenate, returning a new string.
        auto operator+(const std::string& other) const -> cstring;

        /// @brief Concatenate, returning a new string.
        auto operator+(const char* other) const -> cstring;

        /// @brief Append in place.
        auto operator+=(const cstring& other) -> cstring&;

        /// @brief Append in place.
        auto operator+=(const std::string& other) -> cstring&;

        /// @brief Append in place.
        auto operator+=(const char* other) -> cstring&;

        /// @brief Content equality.
        auto operator==(const char* other) const -> bool;

        /// @brief Content equality.
        auto operator==(const std::string& other) const -> bool;

        /// @brief Content equality.
        auto operator==(const cstring& other) const -> bool;

        /// @brief Content inequality.
        auto operator!=(const cstring& other) const -> bool;

        /**
         * @brief The character at @p index.
         * @param index Position within the string.
         * @return A reference to the character.
         * @throws std::invalid_argument if @p index is past the end.
         */
        [[nodiscard]] auto at(const size_t index) const -> char&;

        /// @brief The allocated buffer size (includes the NUL slot).
        [[nodiscard]] auto capacity() const -> size_t;

        /// @brief The character count (excludes the NUL).
        [[nodiscard]] auto length() const -> size_t;

        /// @brief Append one character, growing if needed.
        auto append(const char c) -> void;

        /// @brief Append another string's contents.
        auto append(const cstring& other) -> void;

        /// @brief Append a std::string's contents.
        auto append(const std::string& other) -> void;

        /// @brief Append a C string's contents.
        auto append(const char* c) -> void;

        /// @brief Copy the contents out as a std::string.
        [[nodiscard]] auto to_std_string() const -> std::string;

        [[nodiscard]] auto to_std_string_view() const -> std::string_view;

        /// @brief The backing buffer pointer; valid only until the next mutation.
        [[nodiscard]] auto to_c_string() const -> const char*;

        /// @brief Whether @p str occurs as a substring.
        [[nodiscard]] auto contains(const cstring& str) const -> bool;

        [[nodiscard]] auto begins_with(const cstring& str) const -> bool;

        /// @brief An upper-cased copy.
        [[nodiscard]] auto to_uppper() const -> cstring;

        /// @brief A lower-cased copy.
        [[nodiscard]] auto to_lower() const -> cstring;

        /**
         * @brief A copy with every occurrence of @p oldstr replaced by @p newstr.
         * @return The empty string if @p oldstr is empty.
         */
        [[nodiscard]] auto replace(const cstring& oldstr, const cstring& newstr) const -> cstring;

        /**
         * @brief Split on @p splitstr.
         * @return The pieces; the whole string as a single element if @p splitstr is empty.
         */
        [[nodiscard]] auto split(const cstring& splitstr) const -> std::vector<cstring>;

        /// @brief Whether the text is a valid signed integer literal (optional leading `-`).
        [[nodiscard]] auto is_signed_int() const -> bool;

        /// @brief Whether the text is a valid unsigned integer literal.
        [[nodiscard]] auto is_unsigned_int() const -> bool;

        /**
         * @brief Whether arbitrary TEXT parses as a bool: `true`/`false` in any casing, or `1`/`0`.
         *
         * **This is not the source-literal acceptor** — `ExpressionKeywords::FindBooleanLiteral` is, and
         * it matches the two lowercase spellings exactly. The leniency here belongs to
         * `std::strings::to_bool`, which reads text a program obtained at RUN time (a file, an argument,
         * a user), where `TRUE` and `1` are ordinary spellings of the same intent. While the parser asked
         * this predicate instead, that leniency WAS the language: every casing was a literal, so
         * `i32 TRUE = 5; return TRUE;` handed back 1 with no diagnostic. Keep the two apart — they change
         * for different reasons.
         */
        [[nodiscard]] auto is_bool() const -> bool;

        /// @brief Whether the text is a valid floating-point literal.
        [[nodiscard]] auto is_float() const -> bool;

        /// @brief Whether the text parses as a float without overflow.
        auto is_f32() const -> bool;

        /// @brief Whether the text is a valid double literal.
        auto is_f64() const -> bool;

        /// @brief Whether the text fits an unsigned 8-bit integer.
        auto is_u8() const -> bool;

        /// @brief Whether the text fits an unsigned 16-bit integer.
        auto is_u16() const -> bool;

        /// @brief Whether the text fits an unsigned 32-bit integer.
        auto is_u32() const -> bool;

        /// @brief Whether the text fits an unsigned 64-bit integer.
        auto is_u64() const -> bool;

        /// @brief Whether the text fits a signed 8-bit integer.
        auto is_i8() const -> bool;

        /// @brief Whether the text fits a signed 16-bit integer.
        auto is_i16() const -> bool;

        /// @brief Whether the text fits a signed 32-bit integer.
        auto is_i32() const -> bool;

        /// @brief Whether the text fits a signed 64-bit integer.
        auto is_i64() const -> bool;

        /// @brief Whether the string has no characters.
        [[nodiscard]] auto is_empty() const -> bool;

        /// @brief Parse as an unsigned 8-bit integer.
        auto to_u8() const -> std::uint8_t;

        /// @brief Parse as a signed 8-bit integer.
        auto to_i8() const -> std::int8_t;

        /// @brief Parse as an unsigned 16-bit integer.
        auto to_u16() const -> std::uint16_t;

        /// @brief Parse as a signed 16-bit integer.
        auto to_i16() const -> std::int16_t;

        /// @brief Parse as an unsigned 32-bit integer.
        auto to_u32() const -> std::uint32_t;

        /// @brief Parse as a signed 32-bit integer.
        auto to_i32() const -> std::int32_t;

        /// @brief Parse as an unsigned 64-bit integer.
        [[nodiscard]] auto to_u64() const -> std::uint64_t;

        /// @brief Parse as a signed 64-bit integer.
        [[nodiscard]] auto to_i64() const -> std::int64_t;

        /// @brief Parse as a double.
        [[nodiscard]] auto to_f64() const -> double;

        /// @brief Parse as a float.
        auto to_f32() const -> float;

        /// @brief Parse as a bool, over exactly the set @ref IsBool accepts (throws outside it). Its two
        ///        consumers are `std::strings::to_bool` and the readers of a `bool` literal node's TEXT,
        ///        which `ScalarConstant::ToLiteralText` writes in the canonical lowercase spelling.
        [[nodiscard]] auto to_bool() const -> bool;

        /// @brief A fresh empty string.
        [[nodiscard]] static auto empty() -> cstring;

        /// @brief Render a signed 64-bit integer as a string.
        static auto from_i64(std::int64_t i) -> cstring;

        /// @brief Render an unsigned 64-bit integer as a string.
        static auto from_u64(std::uint64_t u) -> cstring;

        /// @brief Render a raw machine ADDRESS as `0x`-prefixed lower-case hex — how a `rawptr` prints.
        static auto from_address(std::uint64_t address) -> cstring;

        /// @brief Render a double as a string.
        static auto from_f64(double d) -> cstring;

        /// @brief Render a float as a string.
        static auto from_f32(float f) -> cstring;

        /// @brief Render a bool as `"true"`/`"false"`.
        static auto from_bool(bool b) -> cstring;
    };
} // claw

#endif //CLAW_NODESTRING_HPP
