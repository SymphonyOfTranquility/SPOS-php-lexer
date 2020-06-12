//
// Created by art on 6/11/20.
//

#include "Lexer.h"
#include "CharChecks.h"

#include <fstream>
#include <iostream>

namespace lexer
{

    void Lexer::get_dfa_token(CurrentLineState &state)
    {
        std::pair<TokenType, size_t> token_from_dfa = DetFiniteAutomaton::check_value(state.line, state.column);

        tokens.emplace_back(token_from_dfa.first, state.row, state.column);
        state.column = token_from_dfa.second;
        if (token_from_dfa.first == TokenType::INVALID)
            state.column += 1;
        state.token.set_invalid();
    }

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

    void Lexer::handle_multiline_mode(CurrentLineState &state)
    {
        if (state.token.type == TokenType::MultiLineComment)
            get_multiline_comment(state);
        else if (state.token.type == TokenType::StringValueOneQuote)
            get_one_quote_string(state);
        //else if (state.token.type == TokenType::StringValueTwoQuotes)
        //    get_two_quote_string(state);
    }

    void Lexer::handle_number(CurrentLineState &state)
    {
        char c = state.line[state.column];
        if (is_dot(c) && (state.column+1 >= state.line.length() || !is_number(state.line[state.column + 1])))
        {
            get_dfa_token(state);
            return;
        }
        size_t start_pos_number = state.column;
        bool was_dot = false;
        std::string num_value = "";
        bool is_correct = true;
        do
        {
            c = state.line[state.column];
            if (!is_number(c) && !is_dot(c))
            {
                is_correct = false;
                break;
            }
            if (is_dot(c))
            {
                if (was_dot)
                    is_correct = false;
                else
                    was_dot = true;
            }
            if (is_correct)
            {
                num_value += c;
                state.column += 1;
            }
        }
        while (is_correct && state.column < state.line.length());

        if (is_correct || is_correct_after_number(c))
        {
            int index = symbol_table.size();
            symbol_table.push_back(num_value);
            if (was_dot)
                tokens.emplace_back(TokenType::FloatValue, state.row, start_pos_number, index);
            else
                tokens.emplace_back(TokenType::IntValue, state.row, start_pos_number, index);
        }
        else
        {
            size_t first_in_error = state.column;
            while (state.column < state.line.length())
            {
                c = state.line[state.column];
                if (is_correct_after_number(c))
                    break;
                num_value += c;
                state.column += 1;
            }
            invalid_tokens.emplace_back("Unresolved symbol", num_value, state.row, state.column);
        }
    }

    void Lexer::get_next_token(CurrentLineState &state)
    {
        if (state.token.type != TokenType::INVALID)
        {
            handle_multiline_mode(state);
            return;
        }

        while (state.column < state.line[state.column] && is_whitespace(state.line[state.column]))
            state.column += 1;

        if (state.line.length() <= state.column)
            return;
        if (is_number(state.line[state.column]) || is_dot(state.line[state.column]))
            handle_number(state);
        else
            get_dfa_token(state);
    }

    Lexer::Lexer()
    {
        DetFiniteAutomaton::init_dfa_states();
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
            current_state.row += 1;
        }
    }

    void Lexer::output()
    {
        std::cout << "---------------------Tokens---------------------\n";
        for (int i = 0;i < tokens.size(); ++i)
        {
            std::cout.width(10);
            std::cout << TokenValue[static_cast<size_t>(tokens[i].type)];
            std::cout.width(10);
            std::cout << tokens[i].row_pos << "|" << tokens[i].column_pos;

            std::cout.width(15);
            if (tokens[i].symbol_table_index != SYMBOL_TABLE_MAX)
                std::cout << symbol_table[tokens[i].symbol_table_index];
            std::cout << '\n';
        }
        std::cout << "\n---------------------Invalid tokens---------------------\n";
        for (int i = 0;i < invalid_tokens.size(); ++i)
        {
            std::cout << invalid_tokens[i].error_message << " :: in " << invalid_tokens[i].row_pos << "|";
            std::cout << invalid_tokens[i].column_pos << " " << invalid_tokens[i].error_symbol << '\n';
        }
        std::cout << "\n---------------------Symbol table---------------------\n";
        for (int i = 0;i < symbol_table.size(); ++i)
        {
            std::cout << i << " " << symbol_table[i] << '\n';
        }
    }


}