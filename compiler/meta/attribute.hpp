//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_ATTRIBUTE_H
#define SHADE_ATTRIBUTE_H

#include <vector>

#include "../../shared/utils/cstring.hpp"

namespace shade
{
    struct attribute
    {
        cstring name;
        std::vector<cstring> arguments;
    };
} // shade

#endif //SHADE_ATTRIBUTE_H
