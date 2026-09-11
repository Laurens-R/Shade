//
// Created by laure on 11/09/2026.
//

#include "compilecontext.h"

namespace shade {

    const std::vector<diagnostic>& compile_context::get_errors() const {
        return errors;
    }

    const std::vector<diagnostic>& compile_context::get_warnings() const {
        return warnings;
    }

    const std::vector<diagnostic>& compile_context::get_information() const {
        return information;
    }

    const std::vector<diagnostic>& compile_context::get_debug_messages() const {
        return debug_messages;
    }

    const std::vector<cstring>& compile_context::get_lookup_paths() const {
        return lookup_paths;
    }

    void compile_context::log_error(const size_t line, const size_t column, const cstring& message) {
        errors.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_warning(const size_t line, const size_t column, const cstring& message) {
        warnings.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_information(const size_t line, const size_t column, const cstring& message) {
        information.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_debug(const size_t line, const size_t column, const cstring& message) {
        debug_messages.push_back(diagnostic(line, column, message));
    }

    void compile_context::add_lookup_path(const cstring& path) {
        lookup_paths.push_back(path);
    }
}
