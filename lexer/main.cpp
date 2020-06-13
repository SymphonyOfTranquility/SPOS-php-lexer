#include <iostream>
#include "DetFiniteAutomaton.h"
#include "Lexer.h"

int main()
{
    std::cout<< "Hello world!" <<std::endl;

    lexer::Lexer lexer1;
    lexer::DetFiniteAutomaton::output_dfa_states();
    if (lexer1.get_all_tokens("../input.txt"))
        lexer1.output();
    /*
    lexer::DetFiniteAutomaton::init_dfa_states();
    lexer::DetFiniteAutomaton::output_dfa_states();
    std::cout << "Lets check\n";
    std::string code = "=!===<<=<<<--=>>>=<<<=!=<><>11@1=+!++=+==";
    std::cout << code << '\n';
    for (int i = 0;i < code.length(); i++)
    {
        std::pair<lexer::TokenType, size_t> res = lexer::DetFiniteAutomaton::check_value(code, i);
        std::cout << "\t" << i << " " << lexer::TokenValue[static_cast<size_t>(res.first)] << "\t" << res.second << '\n';
        if (res.second != i-1)
            i = res.second;
    }
*/

    return 0;
}
