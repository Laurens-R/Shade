//
// Created by laure on 11/09/2026.
//

#include "compilecontext.hpp"

namespace shade {

    const std::vector<diagnostic>& compile_context::get_errors() const {
        return _errors;
    }

    const std::vector<diagnostic>& compile_context::get_warnings() const {
        return _warnings;
    }

    const std::vector<diagnostic>& compile_context::get_information() const {
        return _information;
    }

    const std::vector<diagnostic>& compile_context::get_debug_messages() const {
        return _debug_messages;
    }

    const std::vector<cstring>& compile_context::get_lookup_paths() const {
        return _lookup_paths;
    }

    program_structure & compile_context::get_program_structure()
    {
        return _types;
    }

    void compile_context::log_error(const size_t line, const size_t column, const cstring& message) {
        _errors.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_warning(const size_t line, const size_t column, const cstring& message) {
        _warnings.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_information(const size_t line, const size_t column, const cstring& message) {
        _information.push_back(diagnostic(line, column, message));
    }

    void compile_context::log_debug(const size_t line, const size_t column, const cstring& message) {
        _debug_messages.push_back(diagnostic(line, column, message));
    }

    void compile_context::add_lookup_path(const cstring& path) {
        _lookup_paths.push_back(path);
    }
}
