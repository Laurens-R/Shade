//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_METADATA_H
#define SHADE_METADATA_H

namespace shade
{
    enum class metadata_type
    {
        function,
        type
    };

    struct metadata
    {
        virtual ~metadata() = default;
        virtual metadata_type get_type() = 0;
    };
} // shade

#endif //SHADE_METADATA_H
