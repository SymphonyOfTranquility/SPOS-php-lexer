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
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
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
}
#endif //LEXER_CHARCHECKS_H
