//
// Created by art on 6/12/20.
//

#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <cstdio>
#include <string>
#include <limits>
#include "TokenTypesList.h"


namespace lexer
{
    const size_t SYMBOL_TABLE_MAX = std::numeric_limits<size_t>::max();

    struct Token {
        TokenType type;
        size_t row_pos;
        size_t column_pos;
        size_t symbol_table_index;

        Token()
        {
            set_invalid();
        }

        Token(TokenType type, size_t row, size_t col, size_t symbol_table_index = SYMBOL_TABLE_MAX)
        {
            this->type = type;
            this->row_pos = row;
            this->column_pos = col;
            this->symbol_table_index = symbol_table_index;
        }

        void set_invalid()
        {
            type = TokenType::INVALID;
            row_pos = column_pos = symbol_table_index = SYMBOL_TABLE_MAX;
        }
    };

    struct InvalidToken
    {
        std::string error_message;
        std::string error_symbol;
        size_t row_pos;
        size_t column_pos;
        size_t symbol_table_index;
        size_t lvl;

        InvalidToken() {}

        InvalidToken(std::string message, std::string symbol, size_t row, size_t col,
                size_t symbol_table_index = SYMBOL_TABLE_MAX, size_t lvl = 0)
        {
            this->error_message = message;
            this->error_symbol = symbol;
            this->row_pos = row;
            this->column_pos = col;
            this->symbol_table_index = symbol_table_index;
            this->lvl = 1;
        }
    };
}

#endif //LEXER_TOKEN_H
