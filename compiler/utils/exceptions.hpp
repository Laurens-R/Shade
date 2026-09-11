//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_EXCEPTIONS_HPP
#define SHADE_EXCEPTIONS_HPP

#include <stdexcept>
#include "../pipeline/compilecontext.h"

namespace shade
{
    class compiler_exception : public std::runtime_error
    {
        std::string message;

    public:
        compiler_exception() : std::runtime_error("Unexpected compiler exception occurred.")
        {
        }

        explicit compiler_exception(const std::string& message) : std::runtime_error(message)
        {
        }

        ~compiler_exception() noexcept override = default;

        [[nodiscard]] const char* what() const noexcept override
        {
            return message.c_str();
        }
    };
}

#endif //SHADE_EXCEPTIONS_HPP
