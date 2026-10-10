#include "Reader.h"
#include "Expression.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " [options and inputs]" << std::endl;
            return 1;
        }

        std::vector<std::string_view> arguments(argv + 1, argv + argc);
        std::vector<std::string> expressions;

        Reader::process_arguments(arguments, expressions);

        std::cout << "Processed Regular Expressions:" << std::endl;
        for (const auto& expr : expressions) {
            std::cout << expr << std::endl;
        }

        Expression expr("(a+b)*(\\emptyset*+\\epsilon).ab");;
        Result result = expr.evaluate();
        for (Symbol symbol : result.starting) {
            symbol.print();
        }
        std::cout << std::endl;
        for (Neighbors neighbors : result.neighbors) {
            neighbors.print();
        }
        std::cout << std::endl;
        for (Symbol symbol : result.ending) {
            symbol.print();
        }
    }

    catch (const Exception& e) {
        std::cerr << e.message() << std::endl;
    }

    return 0;
}
