//
// Created by Laurens Ruijtenberg on 13/09/2026.
//

#include <lexer/lexer.hpp>
#include <parser/parser.hpp>

#include <nanobench.hpp>

int main() {
    constexpr int n = 4096;

    ankerl::nanobench::Bench bench;
    bench.title("shade")
         .unit("lexer and parser")
         .batch(n)          // report per-lookup, not per-loop
         .relative(true);   // first result becomes the 100% baseline

    shade::compile_context context;
    constexpr const char * source = R"(
        struct a {
            func test(i32 a, i32 b) {
                i32 c = a + b;
            }
        }

        struct b {
            func test(i32 a, i32 b) {
                i32 c = a + b;
            }
        }

        struct c {
            func test(i32 a, i32 b) {
                i32 c = a + b;
            }
        }
    )";

    bench.run("parser - first pass (extra small)", [&] {
        auto lex = shade::lexer(source, context);
        const auto tokens = lex.tokenize();

        auto parse = shade::parser(context, tokens);
        auto parse_map = parse.first_pass();
        parse.analyze_first_pass();
    });



    return 0;
}