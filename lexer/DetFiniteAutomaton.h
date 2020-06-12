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
        static void dfs(DFAState &state, token_list_t &token_list, size_t depth);
        static void rec_output(const DFAState &root, size_t depth);
    public:
        static void init_dfa_states();
        static void output_dfa_state();
    };
}


#endif //LEXER_DETFINITEAUTOMATON_H
