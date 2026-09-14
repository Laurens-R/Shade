//
// Created by Laurens Ruijtenberg on 14/09/2026.
//
#include "path_utils.hpp"

namespace shade {
    cstring path_utils::remove_global_prefix(const cstring &path) {
        if (path.begins_with(spelling::modules::module_seperator)) {
            auto path_length = path.length();
            return path.substring(2, path.length());
        }
        return path;
    }

    cstring path_utils::get_path_from_parts(const std::vector<cstring> &parts) {
        bool first = true;
        cstring result = cstring::empty();
        for (auto &part: parts) {
            if (first) {
                result += part;
                first = false;
                continue;
            }

            result += (cstring(spelling::modules::module_seperator));
            result += part;
        }

        return result;
    }

    bool path_utils::is_part_of(const cstring &full_path, const cstring &begin) {
        return full_path.begins_with(begin);
    }

    cstring path_utils::get_relative_path(const cstring &full_path, const cstring &parent_path) {
        cstring final_parent_path = parent_path;

        final_parent_path = remove_global_prefix(final_parent_path);

        if (!final_parent_path.ends_with(spelling::modules::module_seperator)) {
            final_parent_path = final_parent_path + spelling::modules::module_seperator;
        }
        return full_path.replace(final_parent_path, cstring::empty());
    }

    cstring path_utils::get_object_from_path(const cstring &full_path) {
        auto parts = get_parts_from_path(full_path);

        if (!parts.empty()) {
            return parts.back();
        }

        return cstring::empty();
    }

    cstring path_utils::get_parent_namespace_from_path(const cstring &full_path) {
        auto path_parts = get_parts_from_path(full_path);

        if (path_parts.size() <= 1) {
            return cstring::empty();
        }

        path_parts.pop_back();
        return get_path_from_parts(path_parts);
    }

    cstring path_utils::get_global_path() {
        return cstring::empty();
    }
}
