#include <iostream>
#include "DetFiniteAutomaton.h"
int main()
{
    std::cout<< "Hello world!" <<std::endl;
    lexer::DetFiniteAutomaton::init_dfa_states();
    lexer::DetFiniteAutomaton::output_dfa_state();
    return 0;
}
