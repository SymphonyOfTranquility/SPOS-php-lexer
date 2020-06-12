//
// Created by art on 6/11/20.
//

#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <vector>
#include <memory>

#include "Token.h"
#include "DetFiniteAutomaton.h"

namespace lexer
{

    class Lexer
    {
        struct CurrentLineState
        {
            size_t column {0};
            size_t row {0};
            std::string line {""};
            Token token;
            InvalidToken token_error;
        };

        std::vector<std::string> symbol_table;
        std::vector<Token> tokens;
        std::vector<InvalidToken> invalid_tokens;
        CurrentLineState state;

        void get_dfa_token();

        void get_single_line_comment(TokenType type);
        void get_multiline_comment();
        void get_one_quote_string();
        size_t in_key_words(std::string const &word) const;
        void handle_multiline_mode();
        void handle_number();
        void handle_comment();
        void handle_word();
        void handle_single_quote_string();
        void get_next_token();

    public:
        Lexer();
        void get_all_tokens(std::string const &path_to_file);
        void output() const;
    };
}


#endif //LEXER_LEXER_H
