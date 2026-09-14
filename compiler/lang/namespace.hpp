//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_NAMESPACE_H
#define SHADE_NAMESPACE_H

#include <vector>

#include "../utils/cstring.hpp"
#include "spelling.hpp"

namespace shade {
    class namespace_path {
        public:
            static cstring strip_global_prefix(const cstring &path) {
                if (path.begins_with(spelling::modules::module_seperator)) {
                    auto path_length = path.length();
                    return path.substring(2, path.length());
                }
                return path;
            }

            static std::vector<cstring> get_parts(const cstring &path) {
                return path.split(spelling::modules::module_seperator);
            }

            static bool is_part_of(const cstring &full_path, const cstring &begin) {
                return full_path.begins_with(begin);
            }

            static cstring get_relative_path(const cstring &full_path, const cstring &parent_path) {
                cstring final_parent_path = parent_path;

                final_parent_path = strip_global_prefix(final_parent_path);

                if (!final_parent_path.ends_with(spelling::modules::module_seperator)) {
                    final_parent_path = final_parent_path + spelling::modules::module_seperator;
                }
                return full_path.replace(final_parent_path, cstring::empty());
            }

            static cstring get_type_from_path(const cstring &full_path) {
                auto parts = get_parts(full_path);

                if (!parts.empty()) {
                    return parts.back();
                }

                return cstring::empty();
            }

            static cstring get_parent(const cstring & full_path) {
                auto path_parts = get_parts(full_path);

                if (path_parts.size() <= 1) {
                    return cstring::empty();
                }

                path_parts.pop_back();
                return get_path_from_parts(path_parts);
            }

            static cstring get_path_from_parts(const std::vector<cstring> &parts) {
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

            static cstring get_global_path() {
                return cstring::empty();
            }
    };
}

#endif //SHADE_NAMESPACE_H
