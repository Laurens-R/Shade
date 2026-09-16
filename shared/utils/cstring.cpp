#include "../utils/cstring.hpp"

#include <format>
#include <stdexcept>
#include <cstdint>
#include <cstring>

namespace shade {
    constexpr size_t CLAW_STRING_DEFAULTSIZE = 25;

    auto cstring::alloc(size_t capacity, bool cleanMemory) -> void {
        if (_str != nullptr && capacity <= _capacity) {
            return;
        }

        if (capacity >= static_cast<size_t>(PTRDIFF_MAX)) {
            throw std::length_error("CString capacity exceeds maximum object size");
        }

        capacity++; //allow room for the null string terminator

        std::unique_ptr<char[]> new_ptr = std::make_unique<char[]>(capacity);

        if (cleanMemory) {
            std::memset(new_ptr.get(), 0, capacity);
        }

        if (_str) {
#ifndef PLATFORM_WINDOWS
            strcpy(new_ptr.get(), _str.get());
#else
            strncpy_s(new_ptr.get(), capacity, _str.get(), capacity - 1);
#endif
        }

        //swap (not release): new_ptr ends up holding the OLD buffer and frees it on scope exit.
        //Releasing it instead would leak the old buffer on every grow.
        _str.swap(new_ptr);
        _capacity = capacity;
    }

    cstring::cstring() {
        alloc(CLAW_STRING_DEFAULTSIZE);
    }

    cstring::cstring(const char c) {
        alloc(2);
        _str.get()[0]  = c;
        _currentOffset = 1;
    }

    cstring::cstring(const char* initialValue) {
        alloc(std::strlen(initialValue));
#ifndef PLATFORM_WINDOWS
        std::strcpy(_str.get(), initialValue);
#else
        strncpy_s(_str.get(), _capacity, initialValue, _capacity - 1);
#endif
        _currentOffset = std::strlen(initialValue);
    }

    cstring::cstring(const std::string& initialValue) {
        alloc(initialValue.length());
#ifndef PLATFORM_WINDOWS
        std::strcpy(_str.get(), initialValue.c_str());
#else
        strncpy_s(_str.get(), _capacity, initialValue.c_str(), _capacity - 1);
#endif
        _currentOffset = initialValue.length();
    }

    cstring::cstring(const size_t initialLength) {
        alloc(initialLength);
    }

    cstring::cstring(const char* data, const size_t length) {
        alloc(length, /*cleanMemory=*/false);
        std::memcpy(_str.get(), data, length);
        _str.get()[length] = '\0';
        _currentOffset     = length;
    }

    cstring::cstring(const cstring& other) {
        alloc(other.capacity());
#ifndef PLATFORM_WINDOWS
        std::strcpy(_str.get(), other.to_c_string());
#else
        strncpy_s(_str.get(), _capacity, other.to_c_string(), other.length());
#endif
        _currentOffset = other.length();
    }

    cstring::cstring(const cstring& other, const size_t bufferReservation) {
        alloc(bufferReservation);
        const auto otherLen = other.length();
        std::memcpy(_str.get(), other.to_c_string(), otherLen);
        _str.get()[otherLen] = '\0';
        _currentOffset       = otherLen;
    }

    cstring::cstring(cstring&& other) noexcept
        : _capacity(other._capacity),
          _currentOffset(other._currentOffset),
          _str(std::move(other._str)) {
        //Leave `other` in a valid, empty (null-buffer) state: it may only be destroyed or reassigned.
        other._capacity      = 0;
        other._currentOffset = 0;
    }

    auto cstring::operator=(cstring&& other) noexcept -> cstring& {
        if (&other != this) {
            _str           = std::move(other._str); //frees our old buffer, steals theirs — no allocation.
            _capacity      = other._capacity;
            _currentOffset = other._currentOffset;
            other._capacity      = 0;
            other._currentOffset = 0;
        }
        return *this;
    }

    auto cstring::operator=(const cstring& other) -> cstring& {
        if (&other != this) {
            //drop the current buffer first: AllocString copies existing content forward, which would
            //overflow when assigning a SHORTER string onto a longer one. We overwrite it anyway.
            _str.reset();
            _capacity      = 0;
            _currentOffset = 0;

            alloc(other.capacity());
            std::memset(_str.get(), 0, _capacity);

#ifndef PLATFORM_WINDOWS
            std::strcpy(_str.get(), other.to_c_string());
#else
            strncpy_s(_str.get(), _capacity, other.to_c_string(), other.length());
#endif
            _currentOffset = other.length();
        }

        return *this;
    }

    auto cstring::operator=(const char* other) -> cstring& {
        auto len = strlen(other);
        alloc(len);
        std::memset(_str.get(), 0, _capacity);


#ifndef PLATFORM_WINDOWS
        std::strcpy(_str.get(), other);
#else
        strncpy_s(_str.get(), _capacity, other, _capacity - 1);
#endif
        _currentOffset = len;
        return *this;
    }

    auto cstring::operator=(const std::string_view &other) -> cstring&
    {
        auto len = other.length();
        alloc(len);
        std::memset(_str.get(), 0, _capacity);

#ifndef PLATFORM_WINDOWS
        std::strcpy(_str.get(), other.data());
#else
        strncpy_s(_str.get(), _capacity, other.data(), _capacity - 1);
#endif
        _currentOffset = len;
        return *this;
    }

    auto cstring::operator+(const cstring& other) const -> cstring {
        cstring result(*this);
        result.append(other);
        return result;
    }

    auto cstring::operator+(const std::string& other) const -> cstring {
        cstring result(*this);
        result.append(other);
        return result;
    }

    auto cstring::operator+(const char* other) const -> cstring {
        cstring result(*this);
        result.append(other);
        return result;
    }

    auto cstring::operator+=(const cstring& other) -> cstring& {
        append(other);
        return *this;
    }

    auto cstring::operator+=(const std::string& other) -> cstring& {
        append(other);
        return *this;
    }

    auto cstring::operator+=(const char* other) -> cstring& {
        append(other);
        return *this;
    }

    auto cstring::operator==(const char* other) const -> bool {
        return cstring(other) == *this;
    }

    auto cstring::operator==(const std::string& other) const -> bool {
        return cstring(other) == *this;
    }

    auto cstring::operator==(const cstring& other) const -> bool {
        return strcmp(_str.get(), other.to_c_string()) == 0;
    }

    auto cstring::operator!=(const cstring& other) const -> bool {
        return strcmp(_str.get(), other.to_c_string()) != 0;
    }

    auto cstring::at(const size_t index) const -> char& {
        if (index < _currentOffset) {
            return _str.get()[index];
        }

        throw std::invalid_argument("Provided index beyond length of actual string");
    }

    auto cstring::capacity() const -> size_t {
        return _capacity;
    }

    auto cstring::length() const -> size_t {
        return _currentOffset;
    }

    auto cstring::append(const char c) -> void {
        if (_currentOffset + 1 >= _capacity) {
            alloc(_capacity * 2);
        }

        _str.get()[_currentOffset] = c;
        _currentOffset++;
        _str.get()[_currentOffset] = '\0'; //keep NUL-terminated; ToString()/strcpy rely on it
    }

    auto cstring::append(const cstring& other) -> void {
        const auto otherLen = other.length();
        alloc(_currentOffset + otherLen + 1, false);
        std::memcpy(&_str.get()[_currentOffset], other.to_c_string(), otherLen);
        _currentOffset += otherLen;
        _str.get()[_currentOffset] = '\0'; //keep NUL-terminated; ToString()/strcmp rely on it
    }

    auto cstring::append(const std::string& other) -> void {
        const auto converted = cstring(other);
        append(converted);
    }

    auto cstring::append(const char* c) -> void {
        const auto converted = cstring(c);
        append(converted);
    }

    auto cstring::to_std_string() const -> std::string {
        return {_str.get()};
    }

    auto cstring::to_std_string_view() const -> std::string_view
    {
        return {_str.get()};
    }

    auto cstring::to_c_string() const -> const char* {
        return _str.get();
    }

    auto cstring::contains(const cstring& str) const -> bool {
        if (str.length() == 0 || length() == 0) return false;
        return std::strstr(_str.get(), str.to_c_string()) != nullptr;
    }

    auto cstring::begins_with(const cstring& str) const -> bool
    {
        if (str.length() == 0 || length() == 0) return false;
        if (str.length() > length()) return false;

        for (size_t i = 0; i < str.length(); ++i)
        {
            if (str.at(i) != at(i)) return false;
        }

        return true;
    }

    auto cstring::ends_with(const cstring& str) const -> bool {
        if (str.length() == 0 || length() == 0) return false;
        if (str.length() > length()) return false;

        for (size_t i = 0; i < str.length(); ++i)
        {
            if (str.at(i) != at(length() - str.length() + i)) return false;
        }

        return true;
    }

    auto cstring::substring(size_t start, size_t end) const -> cstring {
        return cstring(_str.get() + start, end - start);
    }

    auto cstring::to_uppper() const -> cstring {
        cstring result(_currentOffset);
        for (size_t i = 0; i < _currentOffset; ++i) {
            result.append(static_cast<char>(std::toupper(static_cast<unsigned char>(_str.get()[i]))));
        }
        return result;
    }

    auto cstring::to_lower() const -> cstring {
        cstring result(_currentOffset);
        for (size_t i = 0; i < _currentOffset; ++i) {
            result.append(static_cast<char>(std::tolower(static_cast<unsigned char>(_str.get()[i]))));
        }
        return result;
    }

    auto cstring::replace(const cstring& oldstr, const cstring& newstr) const -> cstring {
        if (oldstr.is_empty()) return cstring::empty();

        if (*this == oldstr) {
            return newstr;
        }

        cstring  result;
        size_t      pos      = 0;
        const char* src      = to_c_string();
        const char* old_cstr = oldstr.to_c_string();
        size_t      old_len  = oldstr.length();
        size_t      src_len  = length();

        while (pos < src_len) {
            const char* found = std::strstr(src + pos, old_cstr);
            if (!found) {
                result.append(src + pos);
                break;
            }
            size_t found_pos = found - src;
            result.append(std::string(src + pos, found_pos - pos));
            result.append(newstr);
            pos = found_pos + old_len;
        }
        return result;
    }

    auto cstring::split(const cstring& splitstr) const -> std::vector<cstring> {
        std::vector<cstring> result;
        if (splitstr.is_empty()) {
            result.push_back(*this);
            return result;
        }

        const char* src       = to_c_string();
        const char* delim     = splitstr.to_c_string();
        size_t      delim_len = splitstr.length();
        size_t      start     = 0;
        size_t      src_len   = length();

        while (start <= src_len) {
            const char* found = std::strstr(src + start, delim);
            if (!found) {
                result.emplace_back(std::string(src + start));
                break;
            }
            size_t found_pos = found - src;
            auto part = std::string(src + start, found_pos - start);
            if (!part.empty()) {
                result.emplace_back(part);
            }
            start = found_pos + delim_len;
        }
        return result;
    }

    auto cstring::is_signed_int() const -> bool {
        const char* str = _str.get();

        if (_currentOffset == 1 && str[0] == '-') return false;

        for (size_t i = 0; i < _currentOffset; ++i) {
            if (i == 0) {
                if (str[i] != '-') {
                    if (!std::isdigit(str[i])) {
                        return false;
                    }
                }
            }
            else {
                if (!std::isdigit(str[i])) {
                    return false;
                }
            }
        }
        return true;
    }

    auto cstring::is_unsigned_int() const -> bool {
        const char* str = _str.get();
        for (size_t i = 0; i < _currentOffset; ++i) {
            if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    auto cstring::is_bool() const -> bool {
        //deliberately lenient, and deliberately NOT what a `.claw` source literal may spell — see the
        //declaration. The source acceptor is ExpressionKeywords::FindBooleanLiteral.
        return this->to_lower() == "true" || this->to_lower() == "false" || *this == "1" || *this == "0";
    }

    auto cstring::is_float() const -> bool {
        //Accepts `[-] digits [. digits] [ (e|E) [+|-] digits ]`. The integer part is REQUIRED (so a
        //leading-dot `.5` is rejected) and a `.` must be followed by at least one digit (so a trailing-dot
        //`5.` is rejected); those forms are deliberately unsupported for readability. A bare integer
        //("123") is still IsFloat()==true — the literal parser checks IsSignedInt/IsUnsignedInt first, so
        //integers classify as integers; float classification only wins when a `.` or exponent is present.
        const char*  str = _str.get();
        const size_t n   = _currentOffset;
        if (n == 0) return false;

        size_t i = 0;
        if (str[0] == '-') {
            i = 1;
            if (n == 1) return false; //a lone '-' is not a number
        }
        //integer part: one or more digits (required — no leading '.').
        size_t intDigits = 0;
        while (i < n && std::isdigit(str[i])) { ++i; ++intDigits; }
        if (intDigits == 0) return false;

        //optional fractional part: '.' MUST be followed by one or more digits (no trailing '.').
        if (i < n && str[i] == '.') {
            ++i;
            size_t fracDigits = 0;
            while (i < n && std::isdigit(str[i])) { ++i; ++fracDigits; }
            if (fracDigits == 0) return false;
        }
        //optional exponent: (e|E) [sign] one-or-more-digits.
        if (i < n && (str[i] == 'e' || str[i] == 'E')) {
            ++i;
            if (i < n && (str[i] == '+' || str[i] == '-')) ++i;
            size_t expDigits = 0;
            while (i < n && std::isdigit(str[i])) { ++i; ++expDigits; }
            if (expDigits == 0) return false;
        }
        return i == n; //the whole string must be consumed
    }

    auto cstring::is_f32() const -> bool {
        if (is_float()) {
            try {
                double value = std::stof(to_c_string());
                return true;
            }
            catch (std::out_of_range& e) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    auto cstring::is_f64() const -> bool {
        if (is_float()) {
            return true;
        }
        else {
            return false;
        }
    }

    auto cstring::is_u8() const -> bool {
        if (is_unsigned_int()) {
            auto value = to_u64();
            if (value <= std::numeric_limits<std::uint8_t>::max()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_u16() const -> bool {
        if (is_unsigned_int()) {
            auto value = to_u64();
            if (value <= std::numeric_limits<std::uint16_t>::max()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_u32() const -> bool {
        if (is_unsigned_int()) {
            auto value = to_u64();
            if (value <= std::numeric_limits<std::uint32_t>::max()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_u64() const -> bool {
        if (is_unsigned_int()) {
            return true;
        }
        return false;
    }

    auto cstring::is_i8() const -> bool {
        if (is_signed_int()) {
            auto value = to_i64();
            if (value <= std::numeric_limits<std::int8_t>::max() && value >= std::numeric_limits<std::int8_t>::min()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_i16() const -> bool {
        if (is_signed_int()) {
            auto value = to_i64();
            if (value <= std::numeric_limits<std::int16_t>::max() && value >= std::numeric_limits<
                std::int16_t>::min()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_i32() const -> bool {
        if (is_signed_int()) {
            auto value = to_i64();
            if (value <= std::numeric_limits<std::int32_t>::max() && value >= std::numeric_limits<
                std::int32_t>::min()) {
                return true;
            }
        }
        return false;
    }

    auto cstring::is_i64() const -> bool {
        if (is_signed_int()) {
            return true;
        }
        return false;
    }

    auto cstring::is_empty() const -> bool {
        return _currentOffset == 0;
    }

    auto cstring::to_u8() const -> std::uint8_t {
        if (is_unsigned_int()) {
            return static_cast<std::uint8_t>(std::stoull(to_c_string()));
        }

        throw std::runtime_error("String is not a valid unsigned int");
    }

    auto cstring::to_i8() const -> std::int8_t {
        if (is_signed_int()) {
            return static_cast<std::int8_t>(std::stoll(to_c_string()));
        }

        throw std::runtime_error("String is not a valid signed int");
    }

    auto cstring::to_u16() const -> std::uint16_t {
        if (is_unsigned_int()) {
            return static_cast<std::uint16_t>(std::stoull(to_c_string()));
        }

        throw std::runtime_error("String is not a valid unsigned int");
    }

    auto cstring::to_i16() const -> std::int16_t {
        if (is_signed_int()) {
            return static_cast<std::int16_t>(std::stoll(to_c_string()));
        }

        throw std::runtime_error("String is not a valid signed int");
    }

    auto cstring::to_u32() const -> std::uint32_t {
        if (is_unsigned_int()) {
            return static_cast<std::uint32_t>(std::stoull(to_c_string()));
        }

        throw std::runtime_error("String is not a valid unsigned int");
    }

    auto cstring::to_i32() const -> std::int32_t {
        if (is_signed_int()) {
            return static_cast<std::int32_t>(std::stoll(to_c_string()));
        }

        throw std::runtime_error("String is not a valid signed int");
    }

    auto cstring::to_u64() const -> std::uint64_t {
        if (is_unsigned_int()) {
            return std::stoull(to_c_string());
        }

        throw std::runtime_error("String is not a valid unsigned int");
    }

    auto cstring::to_i64() const -> std::int64_t {
        if (is_signed_int()) {
            return std::stoll(to_c_string());
        }

        throw std::runtime_error("String is not a valid signed int");
    }

    auto cstring::to_f64() const -> double {
        if (is_float()) {
            return std::stod(to_c_string());
        }

        throw std::runtime_error("String is not a valid double");
    }

    auto cstring::to_f32() const -> float {
        if (is_float()) {
            return std::stof(to_c_string());
        }

        throw std::runtime_error("String is not a valid double");
    }

    auto cstring::to_bool() const -> bool {
        if (is_bool()) {
            return this->to_lower() == "true" || *this == "1";
        }

        throw std::runtime_error("String is not a valid bool");
    }

    auto cstring::empty() -> cstring {
        return cstring();
    }

    auto cstring::from_i64(std::int64_t i) -> cstring {
        return cstring(std::to_string(i));
    }

    auto cstring::from_u64(std::uint64_t u) -> cstring {
        return cstring(std::to_string(u));
    }

    auto cstring::from_address(std::uint64_t address) -> cstring {
        return cstring(std::format("0x{:x}", address));
    }

    auto cstring::from_f64(double d) -> cstring {
        return cstring(std::to_string(d));
    }

    auto cstring::from_f32(float f) -> cstring {
        return cstring(std::to_string(f));
    }

    auto cstring::from_bool(bool b) -> cstring {
        return cstring(std::string(b ? "true" : "false"));
    }
} // claw
