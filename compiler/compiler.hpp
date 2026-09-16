//
// Created by laure on 11/09/2026.
//

#ifndef SHADE_PIPELINE_H
#define SHADE_PIPELINE_H

#include "../shared/utils/cstring.hpp"

namespace shade {
    class compiler {
    public:
            void compile_from_source(cstring source);

        void compile_from_path(const cstring& source_path);
    };
}



#endif //SHADE_PIPELINE_H
