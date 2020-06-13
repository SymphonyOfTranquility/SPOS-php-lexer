//
// Created by art on 6/12/20.
//

#ifndef LEXER_CHARCHECKS_H
#define LEXER_CHARCHECKS_H

#include "Lexer.h"

namespace lexer
{
    bool is_number(char const c)
    {
        return ('0' <= c && c <= '9');
    }

    bool is_symbol(char const c)
    {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
    }

    bool is_dot(char const c)
    {
        return c == '.';
    }

    bool is_whitespace(char const c)
    {
        return c == ' ';
    }

    bool is_punctuation(char const c)
    {
        std::string punctuation = ",;[]{}()";
        return punctuation.find(c) != std::string::npos;
    }

    bool is_operation(char const c)
    {
        std::string operators = "+-*/%^|&=<>!?:";
        return operators.find(c) != std::string::npos;
    }

    bool is_hash_tag_comment(char const c)
    {
        return c == '#';
    }

    bool is_correct_after_number(char const c)
    {
        return is_punctuation(c) || is_hash_tag_comment(c) || is_whitespace(c) || is_operation(c);
    }

    bool is_dollar(char const c)
    {
        return c == '$';
    }

    bool is_comment(char const c1, char const c2)
    {
        return c1 == '/' && (c2 == '/' || c2 == '*');
    }

    bool is_word(char const c)
    {
        return is_symbol(c) || is_number(c);
    }

    bool is_single_quote_string(char const c)
    {
        return c == '\'';
    }

    bool is_double_quote_string(char const c)
    {
        return c == '"';
    }

    bool is_bracket(char const c)
    {
        return c == '[' || c == ']';
    }

    bool is_arrow(char const c1, char const c2)
    {
        return c1 == '-' && c2 == '>';
    }

    bool is_close_brace(char const c)
    {
        return c == '}';
    }

    bool is_tag_after_punctuation(char const c)
    {
        return c == ' ' || c == ';';
    }
}
#endif //LEXER_CHARCHECKS_H
