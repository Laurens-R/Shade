//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_COMPILECONTEXT_H
#define SHADE_COMPILECONTEXT_H

#include <vector>

#include "../utils/cstring.hpp"

namespace shade {
    struct diagnostic {
        size_t line;
        size_t column;
        cstring message;
    };

    class compile_context {
    private:
        std::vector<diagnostic> errors;
        std::vector<diagnostic> warnings;
        std::vector<diagnostic> information;
        std::vector<diagnostic> debug_messages;
        std::vector<cstring> lookup_paths;
    public:
        const std::vector<diagnostic>& get_errors() const;
        const std::vector<diagnostic>& get_warnings() const;
        const std::vector<diagnostic>& get_information() const;
        const std::vector<diagnostic>& get_debug_messages() const;
        const std::vector<cstring>& get_lookup_paths() const;

        void log_error(const size_t line, const size_t column, const cstring & message);
        void log_warning(const size_t line, const size_t column, const cstring & message);
        void log_information(const size_t line, const size_t column, const cstring & message);
        void log_debug(const size_t line, const size_t column, const cstring & message);

        void add_lookup_path(const cstring & path);
    };
}

#endif //SHADE_COMPILECONTEXT_H
