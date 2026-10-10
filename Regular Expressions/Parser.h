#ifndef PARSER_H
#define PARSER_H

#include <string_view>
#include <memory>
#include <stack>
#include <iostream>
#include <string>
#include "Expression.h"
#include "Exception.h"

constexpr char UNKNOWN_TOKEN[] = "Unknown token";
constexpr char OVERFLOW_NUMBER[] = "Overflow number";
constexpr char UNAVAILABLE_MEMORY[] = "Unavailable memory";
constexpr char MISSING_OPERANDS[] = "Missing operands";
constexpr char EMPTY_EXPRESSION[] = "Empty expression";
constexpr char CLOSING_PARENTHESIS_MSG[] = "Unmatched closing parenthesis";
constexpr char OPENING_PARENTHESIS_MSG[] = "Unmatched opening parenthesis";
constexpr char UNUSED_OPERANDS[] = "Unused operands";
constexpr char LEFT_PARENTHESIS = '(';
constexpr char RIGHT_PARENTHESIS = ')';

class Parser {
public:
    [[nodiscard]] static std::unique_ptr<Node> parse(std::string_view input);
private:
    std::stack<char> operators_;
    std::stack<std::unique_ptr<Node>> operands_;

    Parser() = default;

    static bool isOperator(char token);
    static Precedence precedence(char op);
    void checkUnknownToken(char token);
    void pushSymbolNode(char symbol, size_t index);
    void pushEmptyStringNode();
    void pushEmptyLanguageNode();
    void createUnaryOperationNode();
    void createBinaryOperationNode(char op);
    void createOperationNode(char o);
    void processEscapeToken(std::string_view token, bool& lastWasOperand);
    void processAlphabetToken(char token, size_t& currentIndex, bool& lastWasOperand);
    void processLeftParenthesisToken(bool& lastWasOperand);
    void processRightParenthesisToken(bool& lastWasOperand);
    void processIterationToken(bool& lastWasOperand);
    void processBinaryOperatorToken(char token, bool& lastWasOperand);
    void processBinaryToken(char token);
    std::unique_ptr<Node> createExpression(std::string_view input);
    
};

#endif
