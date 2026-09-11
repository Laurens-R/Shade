#ifndef CLAW_TOKENIZER_HPP
#define CLAW_TOKENIZER_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../utils/cstring.hpp"
#include "../pipeline/compilecontext.h"

namespace shade {
    constexpr std::uint32_t CLAW_TOKENIZERSTATE_NONE           = 0b00000;
    constexpr std::uint32_t CLAW_TOKENIZERSTATE_INTOKEN        = 0b00001;
    constexpr std::uint32_t CLAW_TOKENIZERSTATE_INSTRING       = 0b00010;
    /// In a `//` line comment (until the next '\n') or a `/* … */` block comment (until '*/'). While set,
    /// every character is discarded and never tokenized — so a `"` inside a comment can't open a string.
    constexpr std::uint32_t CLAW_TOKENIZERSTATE_INLINECOMMENT  = 0b00100;
    constexpr std::uint32_t CLAW_TOKENIZERSTATE_INBLOCKCOMMENT = 0b01000;

    /**
     * @brief A single lexed token: its text plus the 1-based source position for diagnostics.
     */
    struct language_token {
        size_t  line;
        size_t  column;
        cstring text;

        /// @brief An empty token at position (0, 0).
        language_token()
            : line(0), column(0), text(cstring::empty()) {
        }

        /**
         * @brief A single-character token.
         * @param c            The token character.
         * @param sourceLine   1-based source line.
         * @param sourceColumn 1-based source column.
         */
        language_token(char c, size_t sourceLine, size_t sourceColumn)
            : line(sourceLine), column(sourceColumn), text(c) {
        }

        /**
         * @brief A multi-character token.
         * @param str          The token text.
         * @param sourceLine   1-based source line.
         * @param sourceColumn 1-based source column.
         */
        language_token(const cstring& str, size_t sourceLine, size_t sourceColumn)
            : line(sourceLine), column(sourceColumn), text(str) {
        }
    };

    /**
     * @brief Tokenizes Claw source text into a LanguageToken stream.
     *
     * Both comment forms (line comments and block comments) are discarded during lexing: once a comment
     * opens, every character is thrown away until it closes, so nothing inside a comment is tokenized (a
     * double-quote in a comment can never open a phantom string literal). The terminating newline of a
     * line comment is still emitted as a token.
     */
    class lexer {
    private:
        std::unordered_set<char> _delimiters = {
            ' ', '\t', '\r'
        };

        std::unordered_set<char> _special_symbols = {
            '{', '}', '+', '-', '/', '*', '(', ')', '\'', '|', '&', '$', '!', '?', '@', ';', ':', '[', ']', '\\', '^', '=',
            ',', '\n', '%', '<', '>', '~'
        };

        std::unordered_map<char, std::vector<cstring>> _extended_tokens = {
            {'&', {"&&", "&="}},
            {'|', {"||", "|="}},
            {'=', {"=="}},
            {'<', {"<=", "<-"}}, // `<-` is the move operator (transfer + invalidate source)
            {'>', {">="}},
            {'!', {"!="}},
            {'/', {"/*", "//", "/="}},
            {'*', {"*/", "*="}},
            {'%', {"%="}},
            {'^', {"^="}},
            {'[', {"[]"}},
            {'+', {"++", "+="}},
            {'-', {"--", "-="}},
            {'#', {"#label"}},
            {':', {"::"}},
            // '-' is deliberately NOT in _specialSymbols (a dedicated branch in Tokenize()
            // disambiguates signed numbers), so its compound tokens '--' / '-=' are matched there.
        };

        cstring       _source;
        std::uint32_t _state = CLAW_TOKENIZERSTATE_NONE;
        compile_context & _context;
    public:
        explicit lexer(const cstring& source, compile_context & context);

        /// @brief Tokenize the source into a flat token stream (comments discarded).
        auto tokenize() -> std::vector<language_token>;
    };
}


#endif //CLAW_TOKENIZER_HPP
