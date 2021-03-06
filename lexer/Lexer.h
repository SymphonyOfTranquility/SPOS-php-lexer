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
        };

        std::vector<std::string> symbol_table;
        std::vector<Token> tokens;
        std::vector<InvalidToken> invalid_tokens;
        CurrentLineState state;

        void handle_tag_error(TokenType type, size_t end_pos);
        void get_dfa_token();

        void get_single_line_comment(TokenType type);
        void get_multiline_comment();
        void get_one_quote_string();
        void init_two_quote_string();
        void handle_tokens_in_two_quote();
        void get_two_quote_string();
        size_t in_key_words(std::string const &word) const;
        size_t in_key_variables(std::string const &word) const;
        void handle_multiline_mode();
        void handle_number();
        void handle_comment();
        void handle_word();
        void handle_one_quote_string();
        void handle_two_quote_string();
        void handle_variable();
        void handle_punctuation();
        void get_next_token(bool &is_in_string, bool &normal_end);

    public:
        Lexer();
        bool get_all_tokens(std::string const &path_to_file);
        void output(std::string const &path_to_file) const;
    };
}


#endif //LEXER_LEXER_H
