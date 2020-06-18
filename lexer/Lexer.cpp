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
    void Lexer::handle_tag_error(TokenType type, size_t end_pos)
    {
        std::string word;
        while (state.column < end_pos)
        {
            word += state.line[state.column];
            state.column += 1;
        }
        invalid_tokens.emplace_back("Invalid token ", word, state.row, state.column);
    }
    void Lexer::get_dfa_token()
    {
        std::pair<TokenType, size_t> token_from_dfa = DetFiniteAutomaton::check_value(state.line, state.column);
        if (token_from_dfa.first == TokenType::INVALID)
            handle_tag_error(token_from_dfa.first, token_from_dfa.second);
        else
        {
            tokens.emplace_back(token_from_dfa.first, state.row, state.column);
            state.column = token_from_dfa.second;
        }
    }

    void Lexer::get_single_line_comment(TokenType const type)
    {
        std::string comment = state.line.substr(state.column);
        int index = symbol_table.size();
        symbol_table.push_back(comment);
        tokens.emplace_back(type, state.row, state.column, index);
        state.column = state.line.length();
    }

    void Lexer::get_multiline_comment()
    {
        char c = 0;
        bool prev_is_star;
        do
        {
            prev_is_star = c == '*';
            c = state.line[state.column];
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
        }
        while (state.column < state.line.length() && (!prev_is_star || c != '/'));

        if (prev_is_star && c == '/')
        {
            tokens.push_back(state.token);
            state.token.set_invalid();
        }
    }

    void Lexer::get_one_quote_string()
    {
        char c = 0;
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

    void Lexer::handle_tokens_in_two_quote()
    {
        bool is_in_string = true;
        bool normal_end = false;

        while (state.column < state.line.length() && is_in_string)
        {
            get_next_token(is_in_string, normal_end);
        }

        if (!normal_end)
        {
            if ( state.column < state.line.length())
            {
                std::string symbol;
                symbol += state.line[state.column];
                invalid_tokens.emplace_back("Expected ", symbol, state.row, state.column);
            }
            else if (state.column >= state.line.length())
            {
                std::string symbol;
                symbol += state.line.back();
                invalid_tokens.emplace_back("Expected ", "}", state.row, state.column - 1);
            }
        }
    }

    void Lexer::init_two_quote_string()
    {
        size_t index = symbol_table.size();
        symbol_table.emplace_back("");
        state.token = Token(TokenType::StringValueInTwoQuotes, state.row, state.column, index);
    }
    void Lexer::get_two_quote_string()
    {
        char c = 0;
        bool prev_is_slash;
        do
        {
            prev_is_slash = c == '\\';
            c = state.line[state.column];
            if ((c == '{') && !prev_is_slash && state.column+1 < state.line.length())
            {
                c = state.line[state.column + 1];
                if (is_dollar(c))
                {
                    tokens.push_back(state.token);
                    tokens.emplace_back(TokenType::Concat, state.row, state.column);
                    state.token.set_invalid();
                    handle_tokens_in_two_quote();
                    tokens.emplace_back(TokenType::Concat, state.row, state.column);
                    init_two_quote_string();
                    if (state.column >= state.line.length())
                        break;
                }
                c = state.line[state.column];
            }
            symbol_table[state.token.symbol_table_index] += c;
            state.column += 1;
        }
        while (state.column < state.line.length() && (prev_is_slash || c != '"'));

        if (!prev_is_slash && c == '"')
        {
            symbol_table[state.token.symbol_table_index].pop_back();
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

    size_t Lexer::in_key_variables(const std::string &word) const
    {
        auto start = static_cast<size_t>(TokenType::This);
        auto end = static_cast<size_t>(TokenType::SESSION);
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
        else if (state.token.type == TokenType::StringValueInTwoQuotes)
            get_two_quote_string();
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
            num_value += state.line[state.column];
            symbol_table.push_back(num_value);
            invalid_tokens.emplace_back("Unresolved symbol", num_value, state.row, state.column);
            state.column += 1;
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
    }

    void Lexer::handle_one_quote_string()
    {
        state.token = Token(TokenType::StringValueOneQuote, state.row, state.column, symbol_table.size());
        symbol_table.emplace_back("'");
        state.column += 1;
        get_one_quote_string();
    }


    void Lexer::handle_two_quote_string()
    {
        state.column += 1;
        if (state.column < state.line.length())
        {
            init_two_quote_string();
            get_two_quote_string();
        }
        else
        {
            init_two_quote_string();
            symbol_table[state.token.symbol_table_index] += "\\n";
        }
    }

    void Lexer::handle_variable()
    {
        char c;
        size_t first_in = state.column;
        std::string word;
        do
        {
            c = state.line[state.column];
            if (!is_dollar(c))
                break;
            word += c;
            state.column += 1;
        }
        while (state.column < state.line.length());

        if (state.column == state.line.length() || !is_symbol(state.line[state.column]))
        {
            invalid_tokens.emplace_back("Invalid symbol for var name ", word, state.row, state.column);
            state.column += 1;
            return;
        }

        do
        {
            c = state.line[state.column];
            if (!is_word(c))
                break;
            word += c;
            state.column += 1;
        }
        while (state.column < state.line.length());

        size_t pos = in_key_variables(word);
        if (pos != SYMBOL_TABLE_MAX)
            tokens.emplace_back(static_cast<TokenType>(pos), state.row, first_in);
        else
        {
            size_t index = symbol_table.size();
            symbol_table.push_back(word);
            tokens.emplace_back(TokenType::DollarIdentifier, state.row, first_in, index);
        }
    }

    void Lexer::handle_punctuation()
    {
        std::string word;
        word += state.line[state.column];
        auto start = static_cast<size_t>(TokenType::Comma);
        auto end = static_cast<size_t>(TokenType::RBrace);
        size_t pos = SYMBOL_TABLE_MAX;
        for (size_t i = start;i <= end; ++i)
        {
            if (word == TokenValue[i])
            {
                pos = i;
                break;
            }
        }
        tokens.emplace_back(static_cast<TokenType>(pos), state.row, state.column);
        state.column += 1;
    }

    void Lexer::get_next_token(bool &is_in_string, bool &normal_end)
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
        if (is_in_string)
        {
            if (state.line[state.column] == '"')
            {
                is_in_string = false;
                normal_end = false;
                return;
            }
            if (state.line[state.column] == '}')
            {
                tokens.emplace_back(TokenType::RBrace, state.row, state.column);
                state.column += 1;
                is_in_string = false;
                normal_end = true;
                return;
            }
        }
        char current_symbol = state.line[state.column];
        if (is_number(current_symbol) || is_dot(current_symbol))
        {
            handle_number();
            return;
        }
        if (state.column+1 < state.line.length() &&
            is_comment(current_symbol, state.line[state.column+1]) ||
            is_hash_tag_comment(current_symbol))
        {
            handle_comment();
            return;
        }
        if (is_symbol(current_symbol))
        {
            handle_word();
            return;
        }
        if (is_single_quote_string(current_symbol))
        {
            handle_one_quote_string();
            return;
        }
        if (is_double_quote_string(current_symbol))
        {
            handle_two_quote_string();
            return;
        }
        if (is_dollar(current_symbol))
        {
            handle_variable();
            return;
        }
        if (is_operation(current_symbol))
        {
            get_dfa_token();
            return;
        }
        if (is_punctuation(current_symbol))
        {
            handle_punctuation();
            return;
        }
        std::string symbol;
        symbol += state.line[state.column];
        invalid_tokens.emplace_back("Invalid symbol", symbol, state.row, state.column);
        state.column += 1;
    }

    Lexer::Lexer()
    {
        DetFiniteAutomaton::init_dfa_states();
    }


    bool Lexer::get_all_tokens(std::string const &path_to_file)
    {
        std::ifstream ifs(path_to_file);
        if (!ifs)
        {
            std::cerr << "\tFile doesn't exists!\n";
            return false;
        }
        while (!ifs.eof())
        {
            std::getline(ifs, state.line);
            state.column = 0;
            bool is_in_string = false, normal_end = true;
            while (state.column < state.line.length())
            {
                get_next_token(is_in_string, normal_end);
            }
            state.row += 1;
        }
        if (state.token.type != TokenType::INVALID)
        {
            if (state.token.type == TokenType::StringValueInTwoQuotes)
                invalid_tokens.emplace_back("End of two quote string not exists",
                                            symbol_table.back(), state.row, state.column);
            else if (state.token.type == TokenType::StringValueOneQuote)
                invalid_tokens.emplace_back("End of one quote string not exists",
                                            symbol_table.back(), state.row, state.column);
            else if (state.token.type == TokenType::MultiLineComment)
                invalid_tokens.emplace_back("End of multi line comment not exists",
                                            symbol_table.back(), state.row, state.column);
            else
                invalid_tokens.emplace_back("Unknown error occurred", "", state.row, state.column);
            symbol_table.pop_back();
        }
        return true;
    }

    void Lexer::output(std::string const &path_to_file) const
    {
        std::ofstream ofs(path_to_file);

        ofs << "---------------------------Tokens list---------------------------\n";
        ofs << std::left << std::setw(22) << "Token type";
        ofs << std::left << std::setw(4) << "|row" << "|";
        ofs << std::left << std::setw(6) << "col|";
        ofs << std::left << "symbol table index and value\n";
        ofs << "-----------------------------------------------------------------\n";
        for (int i = 0;i < tokens.size(); ++i)
        {
            if (tokens[i].type == TokenType::DollarIdStartString)
                ofs << '\n';
            ofs << std::left << std::setw(22) << TokenValue[static_cast<size_t>(tokens[i].type)];
            ofs << std::right <<std::setw(4) << tokens[i].row_pos << "|";
            ofs << std::left << std::setw(6) << tokens[i].column_pos;

            //std::cout.width(15);
            if (tokens[i].symbol_table_index != SYMBOL_TABLE_MAX)
                ofs << std::left << tokens[i].symbol_table_index << ") |"
                            << symbol_table[tokens[i].symbol_table_index] << "|";
            ofs << '\n';
            if (tokens[i].type == TokenType::DollarIdEndString)
                ofs << '\n';
        }
        ofs << "\n---------------------------Invalid tokens---------------------------\n";
        ofs << std::left << std::setw(2) << "|row" << "|";
        ofs << std::left << std::setw(8) << "col|";
        ofs << "Error explanation";
        ofs << "\n--------------------------------------------------------------------\n";
        for (int i = 0;i < invalid_tokens.size(); ++i)
        {
            ofs << std::right <<std::setw(2) << invalid_tokens[i].row_pos << "|";
            ofs << std::left << std::setw(8) << invalid_tokens[i].column_pos;
            ofs << invalid_tokens[i].error_message + " |" + invalid_tokens[i].error_symbol + "|\n";
        }
        ofs << "\n---------------------------Symbol table---------------------------\n";
        for (int i = 0;i < symbol_table.size(); ++i)
        {
            ofs << i << ") |" << symbol_table[i] << "|\n";
        }
    }

}