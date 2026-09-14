//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_NAMESPACE_H
#define SHADE_NAMESPACE_H

#include <vector>

#include "../utils/cstring.hpp"
#include "spelling.hpp"

namespace shade {
    class path_utils {
        public:
            static cstring remove_global_prefix(const cstring &path);

            static std::vector<cstring> get_parts_from_path(const cstring &path) {
                return path.split(spelling::modules::module_seperator);
            }

            static cstring get_path_from_parts(const std::vector<cstring> &parts);

            static bool is_part_of(const cstring &full_path, const cstring &begin);

            static cstring get_relative_path(const cstring &full_path, const cstring &parent_path);

            static cstring get_object_from_path(const cstring &full_path);

            static cstring get_parent_namespace_from_path(const cstring & full_path);

            static cstring get_global_path();
    };
}

#endif //SHADE_NAMESPACE_H
