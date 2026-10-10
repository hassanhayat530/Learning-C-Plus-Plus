#include <iostream>
#include "Expression.h"

int main() {
    Node* expr = new SubtractionNode(
        new AdditionNode(
            new NumberNode(7),
            new DivisionNode(
                new SubtractionNode(
                    new NumberNode(9),
                    new MultiplicationNode(
                        new NumberNode(3),
                        new NumberNode(1)
                    )
                ),
                new NumberNode(3)
            )
        ),
        new SubtractionNode(
            new NumberNode(5),
            new NumberNode(1)
        )
    );

    Expression e(expr);
    std::cout << "Infix: ";
    e.print_infix(std::cout);
    std::cout << std::endl;
    std::cout << "Postfix: ";
    e.print_postfix(std::cout);
    std::cout << std::endl;

    return 0;
}

