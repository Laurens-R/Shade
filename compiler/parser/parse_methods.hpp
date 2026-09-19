//
// Created by Laurens Ruijtenberg on 15/09/2026.
//

#ifndef SHADE_CAPTUREMETHODS_HPP
#define SHADE_CAPTUREMETHODS_HPP

#include "../lang/spelling.hpp"
#include "range_parser.hpp"
#include "parser_range_keys.hpp"

namespace shade {
    /**
     * A centralized collection of parser methods for the language.
     */
    struct parse_methods {
        range_parser attributes;
        range_parser modules;
        range_parser structs;
        range_parser functions;
        range_parser if_conditional;
        range_parser if_else_conditional;
        range_parser else_conditional;
        range_parser for_loop;
        range_parser while_loop;
        range_parser raw_scope;
        range_parser statements;
        range_parser type_expressions;
        range_parser function_arguments;

        parse_methods();

        void init();

        static parse_methods & get_instance();
    };
} // shade

#endif //SHADE_CAPTUREMETHODS_HPP
