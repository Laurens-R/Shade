//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_NAMESPACE_H
#define SHADE_NAMESPACE_H

#include <vector>

#include "../utils/cstring.hpp"
#include "spelling.hpp"

namespace shade
{
    class namespace_path
    {
    public:
        static std::vector<cstring> get_parts(const cstring & path)
        {
            return path.split(spelling::modules::module_seperator);
        }

        static bool is_part_of(const cstring & full_path, const cstring & begin)
        {
            return full_path.begins_with(begin);
        }

        static cstring get_relative_path(const cstring & full_path, const cstring & parent_path)
        {
            return full_path.replace(parent_path, cstring::empty());
        }

        static cstring get_type_from_path(const cstring & full_path)
        {
            auto parts = get_parts(full_path);

            if (!parts.empty())
            {
                return parts.back();
            }

            return cstring::empty();
        }

        static cstring get_path_from_parts(const std::vector<cstring> & parts)
        {
            cstring result = cstring::empty();
            for (auto & part : parts)
            {
                result += (cstring(spelling::modules::module_seperator) + part);
            }

            return result;
        }

        static cstring get_global_path()
        {
            return spelling::modules::module_seperator;
        }
    };
}

#endif //SHADE_NAMESPACE_H
