#ifndef PARSER_H
#define PARSER_H

#include <string_view>
#include <cctype>
#include <stack>
#include <string>
#include "Expression.h"

constexpr char UNKOWN_TOKEN[] = "Unknown token";
constexpr char OVERFLOW_NUMBER[] = "Overflow number";
constexpr char UNAVAILABLE_MEMORY[] = "Unavailable memory";
constexpr char MISSING_OPERANDS[] = "Missing operands";
constexpr char EMPTY_EXPRESSION[] = "Empty expression";
constexpr char CLOSING_PARENTHESIS_MSG[] = "Unmatched closing parenthesis";
constexpr char OPENING_PARANTHESIS_MSG[] = "Unmatched opening parenthesis";
constexpr char UNUSED_OPERANDS[] = "Unused operands";

class Parser {
private:
    std::stack<char> operators_;
    std::stack<Node*> operands_;

    Parser() = default;

    bool isOperator(char token) const;
    Precedence precedence(char op) const;
    void cleanUp();
    void checkUnknownToken(char token);
    int parseNumber(std::string_view number);
    void pushNumberNode(std::string_view number);
    void createOperationNode(char o);
    void processLeftParenthesisToken();
    void processRightParenthesisToken();
    void processOperatorToken(char token);
    Node* createExpression(std::string_view input);

public:
    [[nodiscard]] static Node* parse(std::string_view input);
};

#endif // !PARSER_H