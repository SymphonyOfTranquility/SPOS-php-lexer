//
// Created by art on 6/12/20.
//

#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <cstdio>
#include <string>
#include "TokenTypesList.h"


namespace lexer
{
    struct Token {
        TokenType type;
        size_t rowPos;
        size_t columnPos;
        bool isInSymbolTable;
        size_t symbolTableIndex;

        Token(TokenType type, size_t row, size_t col, bool isInSymbolTable, size_t symbolTableIndex)
        {
            this->type = type;
            this->rowPos = row;
            this->columnPos = col;
            this->isInSymbolTable = isInSymbolTable;
            this->symbolTableIndex = symbolTableIndex;
        }
    };

    struct InvalidToken
    {
        std::string errorMessage;
        std::string errorSymbol;
        size_t rowPos;
        size_t columnPos;
        InvalidToken(std::string message, std::string symbol, size_t row, size_t col)
        {
            this->errorMessage = message;
            this->errorSymbol = symbol;
            this->rowPos = row;
            this->columnPos = col;
        }
    };
}

#endif //LEXER_TOKEN_H
