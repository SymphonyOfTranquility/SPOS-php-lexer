//
// Created by art on 6/12/20.
//

#ifndef LEXER_DETFINITEAUTOMATON_H
#define LEXER_DETFINITEAUTOMATON_H

#include <vector>
#include "Token.h"

namespace lexer
{
    class DetFiniteAutomaton
    {
        struct DFAState
        {
            TokenType type = TokenType::INVALID;
            char c = '\0';
            std::vector <DFAState> next_state;
        };

        using token_list_t = std::vector<std::pair<std::string, TokenType> >;
        static DFAState root;
        DetFiniteAutomaton() = default;
        static void init_dfs(DFAState &state, token_list_t const &token_list, size_t const &depth);
        static void rec_output(DFAState const &root, size_t const &depth);
        static std::pair<TokenType, size_t> check_dfs(DFAState const &state, std::string const &code, size_t const pos);
    public:
        static void init_dfa_states();
        static std::pair<TokenType, size_t> check_value(std::string const &code, size_t const &start_pos);
        static void output_dfa_states();
    };
}


#endif //LEXER_DETFINITEAUTOMATON_H
