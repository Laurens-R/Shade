//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_EXCEPTIONS_HPP
#define SHADE_EXCEPTIONS_HPP

#include <stdexcept>
#include "../pipeline/compilecontext.h"

namespace shade {
    class compiler_exception : public std::runtime_error
    {
        std::string message;
    public:
        compiler_exception(std::string message) {
            this->message = message;
        }

        [[nodiscard]] const char* what() const override
        {
            return message.c_str();
        }

        ~compiler_exception() noexcept override;
    };
}

#endif //SHADE_EXCEPTIONS_HPP
