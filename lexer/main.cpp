#include <iostream>
#include "DetFiniteAutomaton.h"
#include "Lexer.h"

int main()
{
    lexer::Lexer lexer1;
    if (lexer1.get_all_tokens("../input.txt"))
    {
        lexer1.output("../log_output.txt");
        std::cout << "Done!";
    }
    return 0;
}
