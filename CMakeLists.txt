cmake_minimum_required(VERSION 4.3)
project(shade)

set(CMAKE_CXX_STANDARD 26)

add_executable(shade main.cpp
        utils/cstring.hpp
        utils/cstring.cpp
        lang/spelling.hpp
        parser/parser.cpp
        parser/parser.h
        parser/range_capture.cpp
        parser/range_capture.h
        lexer/lexer.hpp
        lexer/lexer.cpp
        utils/flagutils.hpp
        pipeline/compilecontext.cpp
        pipeline/compilecontext.h
        pipeline/pipeline.cpp
        pipeline/pipeline.h
        meta/type_information.cpp
        meta/type_information.h
        meta/type_index.cpp
        meta/type_index.h
        utils/exceptions.hpp
        meta/function.cpp
        meta/function.h
        meta/attribute.cpp
        meta/attribute.h
        ast/ast_node.cpp
        ast/ast_node.h
        meta/metadata.cpp
        meta/metadata.h
        lang/ops_and_actions.hpp
        utils/ctmap.hpp
        utils/exceptions.hpp
        meta/module.cpp
        meta/module.hpp)
