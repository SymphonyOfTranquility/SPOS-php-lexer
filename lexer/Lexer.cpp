//
// Created by art on 6/11/20.
//

#include "Lexer.h"
#include "CharChecks.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace lexer
{

    void Lexer::get_dfa_token()
    {
        std::pair<TokenType, size_t> token_from_dfa = DetFiniteAutomaton::check_value(state.line, state.column);

        tokens.emplace_back(token_from_dfa.first, state.row, state.column);
        state.column = token_from_dfa.second;
        if (token_from_dfa.first == TokenType::INVALID)
            state.column += 1;
        state.token.set_invalid();
    }

    void Lexer::get_single_line_comment(TokenType const type)
    {
        std::string comment = state.line.substr(state.column);
        int index = symbol_table.size();
        symbol_table.push_back(comment);
        tokens.emplace_back(type, state.row, state.column, index);
        state.column = state.line.length();
        state.token.set_invalid();
    }

    void Lexer::get_multiline_comment()
    {
        char c;
        bool prev_is_star;
        do
        {
            prev_is_star = c == '*';
            c = state.line[state.column];
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
        }
        while (state.column < state.line.length() && (!prev_is_star || c != '/'));
/*        if (state.column != state.line.length())
        {
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
        }
*/
        if (prev_is_star && c == '/')
        {
            tokens.push_back(state.token);
            state.token.set_invalid();
        }

    }

    void Lexer::get_one_quote_string()
    {
        char c;
        bool prev_is_slash;
        do
        {
            prev_is_slash = c == '\\';
            c = state.line[state.column];
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
        }
        while (state.column < state.line.length() && (prev_is_slash || c != '\''));

        if (!prev_is_slash && c == '\'')
        {
            tokens.push_back(state.token);
            state.token.set_invalid();
        }
        else
        {
            symbol_table[state.token.symbol_table_index] += "\\n";
        }
    }

    size_t Lexer::in_key_words(std::string const &word) const
    {
        auto start = static_cast<size_t>(TokenType::Include);
        auto end = static_cast<size_t>(TokenType::XorWord);
        for (size_t i = start;i <= end; ++i)
        {
            if (word == TokenValue[i])
                return i;
        }
        return SYMBOL_TABLE_MAX;
    }

    void Lexer::handle_multiline_mode()
    {
        if (state.token.type == TokenType::MultiLineComment)
            get_multiline_comment();
        else if (state.token.type == TokenType::StringValueOneQuote)
            get_one_quote_string();
        //else if (state.token.type == TokenType::StringValueTwoQuotes)
        //    get_two_quote_string();
    }

    void Lexer::handle_number()
    {
        char c = state.line[state.column];
        if (is_dot(c) && (state.column+1 >= state.line.length() || !is_number(state.line[state.column + 1])))
        {
            get_dfa_token();
            return;
        }
        size_t start_pos_number = state.column;
        bool was_dot = false;
        std::string num_value;
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

        int index = symbol_table.size();
        if (is_correct || is_correct_after_number(c))
        {
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
            symbol_table.push_back(num_value);
            invalid_tokens.emplace_back("Unresolved symbol", num_value, state.row, first_in_error, index);
        }
    }


    void Lexer::handle_comment()
    {
        char c = state.line[state.column];
        if (is_hash_tag_comment(c))
            get_single_line_comment(TokenType::SingleLineHashTagComment);
        else
        {
            c = state.line[state.column+1];
            if (c == '/')
                get_single_line_comment(TokenType::SingleLineSlashComment);
            else
            {
                int index = symbol_table.size();
                symbol_table.emplace_back("/*");
                state.token = Token(TokenType::MultiLineComment, state.row, state.column, index);
                state.column += 2;

                get_multiline_comment();
            }
        }
    }

    void Lexer::handle_word()
    {
        size_t first_ind = state.column;
        char c;
        std::string word;
        do
        {
            c = state.line[state.column];
            if (!is_word(c))
                break;
            word += c;
            state.column += 1;
        }
        while (state.column < state.line.length());
        std::string lower_case_word = word;
        std::transform(lower_case_word.begin(), lower_case_word.end(), lower_case_word.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        size_t pos = in_key_words(lower_case_word);
        if (pos != SYMBOL_TABLE_MAX)
            tokens.emplace_back(static_cast<TokenType>(pos), state.row, first_ind);
        else
        {
            size_t index = symbol_table.size();
            symbol_table.push_back(word);
            tokens.emplace_back(TokenType::Identifier, state.row, first_ind, index);
        }
        state.token.set_invalid();
    }

    void Lexer::handle_single_quote_string()
    {
        size_t index = symbol_table.size();
        symbol_table.emplace_back("\'");
        state.token = Token(TokenType::StringValueOneQuote, state.row, state.column, index);
        state.column += 1;
        get_one_quote_string();
    }

    void Lexer::get_next_token()
    {
        if (state.token.type != TokenType::INVALID)
        {
            handle_multiline_mode();
            return;
        }

        while (state.column < state.line.length() && is_whitespace(state.line[state.column]))
            state.column += 1;

        if (state.line.length() <= state.column)
            return;

        if (is_number(state.line[state.column]) || is_dot(state.line[state.column]))
        {
            handle_number();
            return;
        }
        if (state.column+1 < state.line.length() &&
            is_comment(state.line[state.column], state.line[state.column+1]) ||
            is_hash_tag_comment(state.line[state.column]))
        {
            handle_comment();
            return;
        }
        if (is_symbol(state.line[state.column]))
        {
            handle_word();
            return;
        }
        if (is_single_quote_string(state.line[state.column]))
        {
            handle_single_quote_string();
            return;
        }
        /*if (is_dollar(state.line[state.column]))
        {
            handle_variable();
            return;
        }*/
        if (is_operation(state.line[state.column]))
        {
            get_dfa_token();
            return;
        }
    }

    Lexer::Lexer()
    {
        DetFiniteAutomaton::init_dfa_states();
    }

    void Lexer::get_all_tokens(std::string const &path_to_file)
    {
        std::ifstream ifs(path_to_file);
        if (!ifs)
        {
            std::cerr << "\tFile doesn't exists!";
            return;
        }
        while (!ifs.eof())
        {
            std::getline(ifs, state.line);
            state.column = 0;
            while (state.column < state.line.length())
            {
                get_next_token();
            }
            state.row += 1;
        }
    }

    void Lexer::output() const
    {
        std::cout << "---------------------Tokens---------------------\n";
        for (int i = 0;i < tokens.size(); ++i)
        {
            std::cout << std::left << std::setw(28) << TokenValue[static_cast<size_t>(tokens[i].type)];
            std::cout << tokens[i].row_pos << "|" << std::left << std::setw(8) << tokens[i].column_pos;

            //std::cout.width(15);
            if (tokens[i].symbol_table_index != SYMBOL_TABLE_MAX)
                std::cout << std::left << tokens[i].symbol_table_index << ") "
                            << symbol_table[tokens[i].symbol_table_index];
            std::cout << '\n';
        }
        std::cout << "\n---------------------Invalid tokens---------------------\n";
        for (int i = 0;i < invalid_tokens.size(); ++i)
        {
            std::cout << invalid_tokens[i].error_message << " in " << invalid_tokens[i].row_pos << "|";
            std::cout << std::left << std::setw(8) << invalid_tokens[i].column_pos;
            std::cout << invalid_tokens[i].symbol_table_index << ") " << invalid_tokens[i].error_symbol << '\n';
        }
        std::cout << "\n---------------------Symbol table---------------------\n";
        for (int i = 0;i < symbol_table.size(); ++i)
        {
            std::cout << i << ") " << symbol_table[i] << '\n';
        }
    }

}