//
// Created by art on 6/11/20.
//

#include "Lexer.h"

#include <fstream>
#include <iostream>

namespace lexer
{

    void Lexer::get_multiline_comment(CurrentLineState &state)
    {
        char c = state.line[state.column];
        bool prev_is_star = false;
        while (state.column < state.line.length() && !(prev_is_star && c == '/'))
        {
            prev_is_star = c == '*';
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
            c = state.line[state.column];
        }
        if (prev_is_star && c == '/')
        {
            tokens.push_back(state.token);
            state.token.set_invalid();
        }
    }

    void Lexer::get_one_quote_string(CurrentLineState &state)
    {
        char c = state.line[state.column];
        bool prev_is_slash = false;
        while (state.column < state.line.length() && !prev_is_slash && c == '\'')
        {
            prev_is_slash = c == '\\';
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
            c = state.line[state.column];
        }
        if (!prev_is_slash && c == '\'')
        {
            tokens.push_back(state.token);
            state.token.set_invalid();
        }
    }

    void Lexer::handle_multiline_mode(CurrentLineState &current_state)
    {
        if (current_state.token.type == TokenType::MultiLineComment)
            get_multiline_comment(current_state);
        else if (current_state.token.type == TokenType::StringValueOneQuote)
            get_one_quote_string(current_state);
        //else if (current_state.token.type == TokenType::StringValueTwoQuotes)
        //    get_two_quote_string(current_state);
    }

    void Lexer::get_next_token(CurrentLineState &current_state)
    {
        if (current_state.token.type != TokenType::INVALID)
        {
            handle_multiline_mode(current_state);
            return;
        }


    }

    void Lexer::get_all_tokens(std::string path_to_file)
    {
        std::ifstream ifs(path_to_file);
        if (!ifs)
        {
            std::cerr << "\tFile doesn't exists!";
            return;
        }
        CurrentLineState current_state;
        while (!ifs.eof())
        {
            std::getline(ifs, current_state.line);
            current_state.column = 0;
            while (current_state.column < current_state.line.length())
            {
                get_next_token(current_state);
            }
        }
    }


}