cmake_minimum_required(VERSION 4.3)
project(shade)

set(CMAKE_CXX_STANDARD 26)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug - build.")
    add_compile_definitions(BUILD_DEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    message("RelWithDebInfo build.")
    add_compile_definitions(BUILD_DEBRELEASE)
else ()
    message("Release build.")
    add_compile_definitions(BUILD_RELEASE)
endif ()

if (WIN32)
    message("Win32 Build")
    add_compile_definitions(PLATFORM_WINDOWS)
endif ()

if(LINUX)
    message("Win32 Build")
    add_compile_definitions(PLATFORM_LINUX)
endif()

if (APPLE)
    message("Apple Build")
    add_compile_definitions(PLATFORM_MACOS)
endif ()

function(claw_configure_warnings target)
    if (MSVC)
        #4100/4101: unreferenced variable, 4804: unsafe bool op, 4189: unused local, 4067: stray tokens.
        target_compile_options(${target} PRIVATE /W4 /wd4100 /wd4101 /wd4804 /wd4189 /wd4067)
    else()
        target_compile_options(${target} PRIVATE
                -Werror
                -Wall
                -Wextra
                -Wshadow
                -Wnon-virtual-dtor
                -Wold-style-cast
                -Wcast-align
                -Wsign-promo
                # -Wno-unused-parameter stays BY DECISION (user, 2026-09-02): the ~15 300 sites are the
                # uniform opcode `Execute(VMState&, VMSharedState&, const VMInstruction&)` signature, an
                # interface, not a defect. The other four suppressions were lifted the same day and
                # their 65 sites fixed at the root.
                -Wno-unused-parameter
        )
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            # "declaration ... changes meaning of X" is GCC-only and default-on (a hard error); the
            # codebase deliberately names a member after its enum type (Diagnostic::Severity).
            target_compile_options(${target} PRIVATE -Wno-changes-meaning)
        else()
            # (Clang's -Wunused-private-field is ON, with -Wall; it was suppressed until 2026-09-02.)
        endif()
    endif()
endfunction()

SET( LIB_SHADE_SOURCES
       compiler/utils/cstring.hpp
       compiler/utils/cstring.cpp
       compiler/lang/spelling.hpp
       compiler/parser/parser.cpp
       compiler/parser/parser.hpp
       compiler/parser/range_parser.cpp
       compiler/parser/range_parser.hpp
       compiler/lexer/lexer.hpp
       compiler/lexer/lexer.cpp
       compiler/utils/flagutils.hpp
       compiler/compilecontext.cpp
       compiler/compilecontext.hpp
       compiler/compiler.cpp
       compiler/compiler.hpp
       compiler/meta/type_definition.cpp
       compiler/meta/type_definition.hpp
       compiler/meta/program_structure.cpp
       compiler/meta/program_structure.hpp
       compiler/utils/exceptions.hpp
       compiler/meta/function_definition.cpp
       compiler/meta/function_definition.hpp
       compiler/meta/attribute.cpp
       compiler/meta/attribute.hpp
       compiler/ast/ast_node.cpp
       compiler/ast/ast_node.hpp
       compiler/meta/metadata.cpp
       compiler/meta/metadata.hpp
       compiler/lang/ops_and_actions.hpp
       compiler/utils/ctmap.hpp
       compiler/utils/exceptions.hpp
       compiler/meta/module_definition.cpp
       compiler/meta/module_definition.hpp
       compiler/lang/path_utils.hpp
        compiler/parser/parser_range_keys.hpp
        compiler/lang/path_utils.cpp
        compiler/parser/structparser.cpp
        compiler/parser/structparser.hpp
        compiler/parser/typeexpressionparser.cpp
        compiler/parser/typeexpressionparser.hpp
        compiler/parser/parse_methods.cpp
        compiler/parser/parse_methods.hpp
        shared/value.hpp
        runtime/vm/vmstate.hpp
        shared/instruction.hpp
)

add_library(lib_shade OBJECT ${LIB_SHADE_SOURCES})
target_include_directories(lib_shade PUBLIC compiler)

add_executable (shade main.cpp)
target_include_directories(shade PUBLIC compiler)
target_link_libraries(shade PRIVATE lib_shade)

add_subdirectory(vendor/googletest)
enable_testing()
add_executable(tests tests/parser/namespace_tests.cpp
        tests/parser/cstring_tests.cpp
        tests/parser/lexer_tests.cpp
        tests/parser/tests_common.hpp
        tests/parser/parser_tests.cpp
        tests/parser/range_capture_test.cpp
        tests/parser/parser_scan_tests.cpp
        tests/parser/module_definition_tests.cpp
        tests/parser/type_expression_parser_tests.cpp)
target_include_directories(tests PUBLIC compiler)
target_link_libraries(tests PRIVATE lib_shade)
target_link_libraries(tests PRIVATE GTest::gtest_main )

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(benchmark benchmark/main.cpp
                         benchmark/nanobench.cpp)
target_include_directories(benchmark PUBLIC compiler)
target_include_directories(benchmark PUBLIC vendor/nanobench)
target_link_libraries(benchmark PRIVATE lib_shade)
