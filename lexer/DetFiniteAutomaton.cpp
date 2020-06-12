//
// Created by art on 6/12/20.
//

#include <algorithm>
#include <iostream>
#include "DetFiniteAutomaton.h"

namespace lexer
{
    DetFiniteAutomaton::DFAState DetFiniteAutomaton::root;

    void DetFiniteAutomaton::init_dfs(DFAState &state, token_list_t const &token_list, size_t const &depth)
    {
        size_t i = 0;
        do
        {
            if (token_list[i].first.length() > depth)
            {
                token_list_t next_list;
                DFAState next_state;
                if (depth+1 == token_list[i].first.length())
                    next_state.type = token_list[i].second;
                else
                {
                    next_state.type = TokenType::INVALID;
                    next_list.push_back(token_list[i]);
                }
                next_state.c = token_list[i].first[depth];

                size_t j = i+1;
                while (j < token_list.size() && token_list[j].first.length() > depth + 1)
                {
                    if (next_state.c == token_list[j].first[depth])
                        next_list.push_back(token_list[j]);
                    else
                        break;
                    ++j;
                }
                if (!next_list.empty())
                    init_dfs(next_state, next_list, depth + 1);

                state.next_state.push_back(std::move(next_state));
                i = j;
            }
        }
        while (i < token_list.size());
    }

    void DetFiniteAutomaton::rec_output(DFAState const &root, size_t const &depth)
    {
        for (size_t i = 0;i < depth; ++i)
            std::cout << "  ";
        std::cout << root.c << " " << TokenValue[static_cast<size_t>(root.type)] << "\n";
        for (size_t i = 0;i < root.next_state.size(); ++i)
            rec_output(root.next_state[i], depth+1);
    }

    std::pair<TokenType, size_t> DetFiniteAutomaton::check_dfs(DFAState const &state, std::string const &code, size_t const pos)
    {
        if (code.length() <= pos)
            return { state.type, pos-1 };

        for (int i = 0;i < state.next_state.size(); ++i)
            if (state.next_state[i].c == code[pos])
                return check_dfs(state.next_state[i], code, pos+1);
        return { state.type, pos - 1};
    }

    void DetFiniteAutomaton::init_dfa_states()
    {
        token_list_t dfa_tokens_list;
        size_t start = static_cast<size_t>(TokenType::Add);
        size_t end = static_cast<size_t>(TokenType::SingleLineSlashComment);
        for (size_t i = start;i < end; ++i)
            dfa_tokens_list.push_back( { TokenValue[i], static_cast<TokenType>(i) } );
        std::sort(dfa_tokens_list.begin(), dfa_tokens_list.end());

        init_dfs(root, dfa_tokens_list, 0);
        std::cout << dfa_tokens_list.size();
    }


    std::pair<TokenType, size_t> DetFiniteAutomaton::check_value(std::string const &code, size_t const &start_pos)
    {
        return check_dfs(root, code, start_pos);
    }

    void DetFiniteAutomaton::output_dfa_states()
    {
        rec_output(root, 0);
    }

}