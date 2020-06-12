//
// Created by art on 6/11/20.
//

#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <vector>
#include <memory>
#include "Token.h"

namespace lexer
{
    struct CurrentLineState
    {
        size_t column {0};
        size_t row {0};
        std::string line {""};
        Token token;
        InvalidToken token_error;
    };

    class Lexer
    {
        std::vector<std::string> symbol_table;
        std::vector<Token> tokens;
        std::vector<InvalidToken> invalid_tokens;

        void get_multiline_comment(CurrentLineState &state);
        void get_one_quote_string(CurrentLineState &state);
        void handle_multiline_mode(CurrentLineState &current_state);
        void get_next_token(CurrentLineState &current_state);

    public:
        Lexer() {};
        void get_all_tokens(std::string path_to_file);
    };
}


#endif //LEXER_LEXER_H
