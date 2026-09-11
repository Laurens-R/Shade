#include "lexer.hpp"

#include "../utils/flagutils.hpp"

#include <string>

namespace shade
{
    lexer::lexer(const cstring& source, compile_context & context)
        : _context(context)
    {
        _source = source;
    }

    auto lexer::tokenize() -> std::vector<language_token>
    {
        std::vector<language_token> tokens;
        language_token current_token;

        size_t line_counter = 1;
        size_t column_counter = 1;

        for (size_t index = 0; index < _source.length(); ++index)
        {
            if (current_token.text.length() == 0)
            {
                current_token.line = line_counter;
                current_token.column = column_counter;
            }
            const char commentChar = _source.at(index);

            if (is_flag_set(_state, CLAW_TOKENIZERSTATE_INLINECOMMENT))
            {
                if (commentChar != '\n')
                {
                    column_counter++;
                    continue;
                }

                _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INLINECOMMENT);
            }
            else if (is_flag_set(_state, CLAW_TOKENIZERSTATE_INBLOCKCOMMENT))
            {
                if (commentChar == '*' && index + 1 < _source.length() && _source.at(index + 1) == '/')
                {
                    _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INBLOCKCOMMENT);
                    column_counter += 2;
                    ++index;
                    continue;
                }

                if (commentChar == '\n')
                {
                    line_counter++;
                    column_counter = 1;
                }
                else
                {
                    column_counter++;
                }

                continue;
            }
            else if (
                !is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING)
                && commentChar == '/'
                && index + 1 < _source.length()
                && (_source.at(index + 1) == '/' || _source.at(index + 1) == '*')
            )
            {
                if (current_token.text.length() > 0)
                {
                    tokens.push_back(current_token);
                    current_token = {};
                }

                _state = set_flag(_state,
                                  _source.at(index + 1) == '/'
                                      ? CLAW_TOKENIZERSTATE_INLINECOMMENT
                                      : CLAW_TOKENIZERSTATE_INBLOCKCOMMENT);

                _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                column_counter += 2;
                ++index;
                continue;
            }

            if (char c = _source.at(index); _delimiters.contains(c) && !
                is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING))
            {
                _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                if (current_token.text.length() > 0)
                {
                    tokens.push_back(current_token);
                    current_token = {};
                    current_token.line = line_counter;
                    current_token.column = column_counter;
                }
            }
            else
            {
                if (c == '\\' && is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING) && index + 1 < _source.length())
                {
                    char decoded = 0;
                    switch (_source.at(index + 1))
                    {
                    case 'n': decoded = '\n';
                        break;
                    case 't': decoded = '\t';
                        break;
                    case 'r': decoded = '\r';
                        break;
                    case '0': decoded = '\0';
                        break;
                    case '\\': decoded = '\\';
                        break;
                    case '"': decoded = '"';
                        break;
                    case '\'': decoded = '\'';
                        break;
                    default: decoded = _source.at(index + 1);
                        break;
                    }
                    current_token.text.append(decoded);
                    ++index; //consume the escape character (the trailing tracker advances past it)
                    column_counter++; //account for the backslash itself (the escape char is tracked below)
                    continue;
                }
                // A '+' / '-' immediately after a numeric mantissa's exponent marker (the sign in
                // `1.5e-3` / `2e+8`) is part of the FLOAT LITERAL, not an operator — append it to the
                // number being built, BEFORE the special-symbol dispatch below would flush the token and
                // emit a '-' / '+' operator. Guarded tightly to a mantissa-then-exponent shape: the token
                // so far starts with a digit (numeric, not an identifier) and ends with 'e'/'E' preceded
                // by a digit. So `a-1`, `5-3`, `0xabce-1` are untouched; only `<digits>[.<digits>]e` is.
                if ((c == '+' || c == '-') && !is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING)
                    && current_token.text.length() >= 2)
                {
                    const size_t len = current_token.text.length();
                    const char first = current_token.text.at(0);
                    const char last = current_token.text.at(len - 1);
                    const char prev = current_token.text.at(len - 2);

                    if (std::isdigit(static_cast<unsigned char>(first)) && (last == 'e' || last == 'E')
                        && std::isdigit(static_cast<unsigned char>(prev)))
                    {
                        current_token.text.append(c);
                        column_counter++; //consumed here; the loop's bottom column tracker is skipped by continue
                        continue;
                    }
                }
                if (_special_symbols.contains(c) && !is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING))
                {
                    if (c == '\'')
                    {
                        bool valid = false;
                        char value = 0;
                        size_t consumed = 0; //source chars after the opening quote, incl. the closing quote
                        if (index + 3 < _source.length() && _source.at(index + 1) == '\\' && _source.at(index + 3) == '\'')
                        {
                            switch (_source.at(index + 2))
                            {
                            case 'n': value = '\n';
                                break;
                            case 't': value = '\t';
                                break;
                            case 'r': value = '\r';
                                break;
                            case '0': value = '\0';
                                break;
                            case '\\': value = '\\';
                                break;
                            case '\'': value = '\'';
                                break;
                            case '"': value = '"';
                                break;
                            default: value = _source.at(index + 2);
                                break;
                            }
                            consumed = 3; //backslash, escape char, closing quote
                            valid = true;
                        }
                        else if (index + 2 < _source.length() && _source.at(index + 1) != '\'' &&
                            _source.at(index + 1) != '\\' && _source.at(index + 2) == '\'')
                        {
                            value = _source.at(index + 1);
                            consumed = 2; //the character and the closing quote
                            valid = true;
                        }
                        if (valid)
                        {
                            if (current_token.text.length() > 0)
                            {
                                tokens.push_back(current_token);
                                current_token = {};
                            }
                            tokens.emplace_back(
                                cstring(std::to_string(static_cast<int>(static_cast<unsigned char>(value)))),
                                line_counter,
                                column_counter);
                            index += consumed;
                            _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                            continue;
                        }
                        //not a char literal: fall through to the ordinary special-symbol handling below.
                    }

                    if (current_token.text.length() > 0)
                    {
                        tokens.push_back(current_token);
                    }

                    current_token = {};

                    // A special symbol may extend into a longer multi-char token (e.g. '=' -> '==').
                    bool wasExtendedToken = false;
                    if (_extended_tokens.contains(c))
                    {
                        auto potentialMatches = _extended_tokens[c];
                        for (auto& match : potentialMatches)
                        {
                            cstring potentialToken(c);
                            size_t matchLength = match.length();

                            if (index + matchLength > _source.length())
                            {
                                continue;
                            }

                            for (size_t matchIndex = 1; matchIndex < matchLength; ++matchIndex)
                            {
                                potentialToken.append(_source.at(index + matchIndex));
                            }

                            if (potentialToken == match)
                            {
                                tokens.emplace_back(match, line_counter, column_counter);
                                index += (matchLength - 1);
                                wasExtendedToken = true;
                                break;
                            }
                        }
                    }

                    if (!wasExtendedToken)
                    {
                        tokens.emplace_back(c, line_counter, column_counter);
                    }

                    _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                }
                else if (c == '"')
                {
                    if (!is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING))
                    {
                        _state = set_flag(_state, CLAW_TOKENIZERSTATE_INSTRING);

                        if (current_token.text.length() > 0)
                        {
                            tokens.push_back(current_token);
                            current_token = {};
                            current_token.line = line_counter;
                            current_token.column = column_counter;
                        }

                        tokens.emplace_back(c, line_counter, column_counter);
                    }
                    else
                    {
                        _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INSTRING);
                        tokens.push_back(current_token);
                        tokens.emplace_back(c, line_counter, column_counter);
                        current_token = {};
                        current_token.line = line_counter;
                        current_token.column = column_counter;
                    }
                }
                else if (c == '-')
                {
                    // '-' is handled specially to distinguish a signed-number sign from the operator.
                    // this cannot be otherwise captured by the default rule definitions
                    if (!is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING))
                    {
                        if (index + 1 < _source.length() && (_source.at(index + 1) == '-' || _source.at(index + 1) == '='))
                        {
                            if (current_token.text.length() > 0)
                            {
                                tokens.push_back(current_token);
                            }

                            cstring twoChar(c);
                            twoChar.append(_source.at(index + 1));
                            tokens.emplace_back(twoChar, line_counter, column_counter);

                            current_token = {};
                            current_token.line = line_counter;
                            current_token.column = column_counter;
                            _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);

                            ++index;
                        }
                        else if (index + 1 < _source.length() && std::isdigit(_source.at(index + 1)))
                        {
                            _state = set_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                            current_token.text.append(c);
                        }
                        else
                        {
                            if (current_token.text.length() > 0)
                            {
                                tokens.push_back(current_token);
                            }

                            tokens.emplace_back(c, line_counter, column_counter);
                            current_token = {};
                            current_token.line = line_counter;
                            current_token.column = column_counter;
                            _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                        }
                    }
                    else
                    {
                        //inside a string literal '-' is an ordinary character.
                        current_token.text.append(c);
                    }
                }
                else if (c == '.')
                {
                    //decimal handling
                    if (!is_flag_set(_state, CLAW_TOKENIZERSTATE_INSTRING))
                    {
                        if (index + 1 < _source.length() && std::isdigit(_source.at(index + 1)))
                        {
                            _state = set_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                            current_token.text.append(c);
                        }
                        else
                        {
                            if (current_token.text.length() > 0)
                            {
                                tokens.push_back(current_token);
                            }

                            tokens.emplace_back(c, line_counter, column_counter);
                            current_token = {};
                            current_token.line = line_counter;
                            current_token.column = column_counter;
                            _state = clear_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                        }
                    }
                    else
                    {
                        //inside a string literal '.' is an ordinary character.
                        current_token.text.append(c);
                    }
                }
                else
                {
                    _state = set_flag(_state, CLAW_TOKENIZERSTATE_INTOKEN);
                    current_token.text.append(c);
                }
            }

            if (_source.at(index) == '\n')
            {
                line_counter++;
                column_counter = 1;
            }
            else
            {
                column_counter++;
            }
        }

        if (current_token.text.length() > 0)
        {
            tokens.push_back(current_token);
        }

        return tokens;
    }
}
