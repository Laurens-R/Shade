//
// Created by laure on 11/09/2026.
//

#include "pipeline.h"

#include "../lexer/lexer.hpp"
#include "../parser/parser.h"

namespace shade {

    void pipeline::compile(const cstring& source_path) {

        cstring source = cstring::empty(); //todo load claw source

        //phase 1: pre-process

        //phase 2: lex into tokens
        lexer lex = lexer(source);
        auto tokens = lex.tokenize();

        //phase 3: parse tokens into AST
        parser::parse(tokens, 0, tokens.size() - 1);

        //phase 4: optimize AST

        //phase 5: generate bytecode

        //phase 6: bytecode optimizer

        //phase 7: AOT compile into executable image
    }
}
