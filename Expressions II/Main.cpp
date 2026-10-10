#include <iostream>
#include "Expression.h"
#include "Parser.h"
#include "Exception.h"


int main() {
    try {
        std::string_view input = "(1*2)";

        // Convert to postfix using Parser.
        std::cout << "Postfix Notation: ";
        // Parser::convert_to_postfix(input, std::cout);
        std::cout << std::endl;

        // Build the syntax tree and evaluate.
        Expression expr(input);
        std::cout << "Evaluated Result: " << expr.evaluate() << std::endl;

        // Print the expression in infix and postfix notations.
        std::cout << "Infix Notation: ";
        expr.print_infix();
        std::cout << std::endl;

        std::cout << "Postfix Notation (from tree): ";
        expr.print_postfix();
        std::cout << std::endl;
    }
    catch (const ParseException& e) {
        std::cerr << "Parse error: " << e.message() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    return 0;
}



