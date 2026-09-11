//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_PIPELINE_H
#define SHADE_PIPELINE_H

#include "../utils/cstring.hpp"

namespace shade {
    class pipeline {
    public:
        void compile(const cstring& source_path);
    };
}



#endif //SHADE_PIPELINE_H
