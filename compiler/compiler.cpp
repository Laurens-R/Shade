//
// Created by laure on 11/09/2026.
//

#include "compiler.hpp"
#include "compilecontext.hpp"

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"


namespace shade {
    void compiler::compile_from_source(cstring source) {
        compile_context context;
        //phase 1: pre-process

        //phase 2: lex into tokens
        auto lex = lexer(source, context);
        const auto tokens = lex.tokenize();

        //phase 3: parse tokens into AST
        auto parser = shade::parser(context, tokens);

        //first parse pass: get a overall code map by an initial parse of all the source sections
        parser.capture_main_areas();

        //second parse pass: scan through the map to register all found types (structs) this will allow us to resolve type references
        //without requiring source-order declarations during the in-depth parse pass
        parser.analyze_main_areas();

        //phase 4: optimize AST

        //phase 5: generate bytecode

        //phase 6: bytecode optimizer

        //phase 7: AOT compile into executable image
    }

    void compiler::compile_from_path(const cstring& source_path) {

        cstring source = cstring::empty(); //todo load claw source
        compile_from_source(source);
    }
}
