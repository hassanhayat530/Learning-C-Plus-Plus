#include <stack>
#include "Parser.h"
#include "Expression.h"

std::unique_ptr<Node> Parser::parse(std::string_view input) {
    try {
        Parser parser;
        return parser.createExpression(input);
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

Precedence Parser::precedence(char op) {
    switch (op) {
    case ITERATION_OPERATOR:
        return Precedence::HIGH;
    case CONCATENATION_OPERATOR:
        return Precedence::MID;
    case ALTERNATION_OPERATOR:
        return Precedence::LOW;
    default:
        return Precedence::NULL_;
    }
}

bool Parser::isOperator(char token) {
    return token == ITERATION_OPERATOR || token == CONCATENATION_OPERATOR || token == ALTERNATION_OPERATOR;
}

void Parser::checkUnknownToken(char token) {
    if (isOperator(token) || token == LEFT_PARENTHESIS || token == RIGHT_PARENTHESIS ||
        std::isalpha(token) || token == SPACE || token == SLASH_CHAR) {
        return;
    }
    else {
        throw ParseException(UNKNOWN_TOKEN);
    }
}

void Parser::pushSymbolNode(char symbol, size_t index) {
    try {
        operands_.push(std::make_unique<SymbolNode>(Symbol(symbol, index)));
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::pushEmptyStringNode() {
    try {
        operands_.push(std::make_unique<EmptyStringNode>());
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::pushEmptyLanguageNode() {
    try {
        operands_.push(std::make_unique<EmptyLanguageNode>());
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::createUnaryOperationNode() {
    if (operands_.empty()) {
        throw ParseException(MISSING_OPERANDS);
    }
    auto operand = std::move(operands_.top());
    operands_.pop();
    try {
        operands_.push(std::make_unique<IterationNode>(std::move(operand)));
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::createBinaryOperationNode(char op) {
    if (operands_.size() < 2) {
        throw ParseException(MISSING_OPERANDS);
    }
    auto right = std::move(operands_.top());
    operands_.pop();
    auto left = std::move(operands_.top());
    operands_.pop();

    try {
        std::unique_ptr<Node> operation;
        if (op == CONCATENATION_OPERATOR) {
            operation = std::make_unique<ConcatenationNode>(std::move(left), std::move(right));
        }
        else if (op == ALTERNATION_OPERATOR) {
            operation = std::make_unique<AlternationNode>(std::move(left), std::move(right));
        }
        operands_.push(std::move(operation));
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::createOperationNode(char op) {
    if (op == ITERATION_OPERATOR) {
        createUnaryOperationNode();
    }
    else {
        createBinaryOperationNode(op);
    }
}

void Parser::processEscapeToken(std::string_view token, bool& lastWasOperand) {
    if (lastWasOperand) {
        processBinaryToken(CONCATENATION_OPERATOR);
    }

    if (token == EMPTY_STRING) {
        pushEmptyStringNode();
    }
    else if (token == EMPTY_SET) {
        pushEmptyLanguageNode();
    }

    lastWasOperand = true;
}

void Parser::processAlphabetToken(char token, size_t& currentIndex, bool& lastWasOperand) {
    if (lastWasOperand) {
        processBinaryToken(CONCATENATION_OPERATOR);
    }
    pushSymbolNode(token, currentIndex++);
    lastWasOperand = true;
}

void Parser::processLeftParenthesisToken(bool& lastWasOperand) {
    if (lastWasOperand) {
        processBinaryToken(CONCATENATION_OPERATOR);
    }
    operators_.push(LEFT_PARENTHESIS);
    lastWasOperand = false;
}

void Parser::processRightParenthesisToken(bool& lastWasOperand) {
    while (!operators_.empty() && operators_.top() != LEFT_PARENTHESIS) {
        createOperationNode(operators_.top());
        operators_.pop();
    }
    if (operators_.empty()) {
        throw ParseException(CLOSING_PARENTHESIS_MSG);
    }
    operators_.pop();
    lastWasOperand = true;
}

void Parser::processIterationToken(bool& lastWasOperand) {
    createUnaryOperationNode();
    lastWasOperand = true;
}

void Parser::processBinaryOperatorToken(char token, bool& lastWasOperand) {
    while (!operators_.empty() &&
        operators_.top() != LEFT_PARENTHESIS &&
        precedence(operators_.top()) >= precedence(token)) {
        createOperationNode(operators_.top());
        operators_.pop();
    }
    operators_.push(token);
    lastWasOperand = false;
}

std::unique_ptr<Node> Parser::createExpression(std::string_view input) {
    if (input.empty()) {
        throw ParseException(EMPTY_EXPRESSION);
    }

    bool parsingEscapedToken = false;
    size_t escapedTokenStart = 0;
    size_t currentIndex = 1;
    bool lastWasOperand = false;

    const std::string_view emptyStringView(EMPTY_STRING);
    const std::string_view emptySetView(EMPTY_SET);

    for (size_t i = 0; i < input.length(); ++i) {
        char token = input[i];

        if (token == SPACE) {
            continue;
        }

        checkUnknownToken(token);

        if (token == SLASH_CHAR && !parsingEscapedToken) {
            parsingEscapedToken = true;
            escapedTokenStart = i;
            continue;
        }

        if (parsingEscapedToken) {
            size_t currentLength = i - escapedTokenStart + 1;

            if (currentLength == emptyStringView.length() &&
                input.substr(escapedTokenStart, emptyStringView.length()) == emptyStringView) {
                processEscapeToken(EMPTY_STRING, lastWasOperand);
                i = escapedTokenStart + emptyStringView.length() - 1;
                parsingEscapedToken = false;
            }
            else if (currentLength == emptySetView.length() &&
                input.substr(escapedTokenStart, emptySetView.length()) == emptySetView) {
                processEscapeToken(EMPTY_SET, lastWasOperand);
                i = escapedTokenStart + emptySetView.length() - 1;
                parsingEscapedToken = false;
            }
            else if (currentLength > emptyStringView.length() && currentLength > emptySetView.length()) {
                throw ParseException(UNKNOWN_TOKEN);
            }
            continue;
        }

        if (std::isalpha(token)) {
            processAlphabetToken(token, currentIndex, lastWasOperand);
        }
        else if (token == LEFT_PARENTHESIS) {
            processLeftParenthesisToken(lastWasOperand);
        }
        else if (token == RIGHT_PARENTHESIS) {
            processRightParenthesisToken(lastWasOperand);
        }
        else if (token == ITERATION_OPERATOR) {
            processIterationToken(lastWasOperand);
        }
        else if (isOperator(token)) {
            processBinaryOperatorToken(token, lastWasOperand);
        }
    }

    if (parsingEscapedToken) {
        throw ParseException(UNKNOWN_TOKEN);
    }

    while (!operators_.empty()) {
        if (operators_.top() == LEFT_PARENTHESIS) {
            throw ParseException(OPENING_PARENTHESIS_MSG);
        }
        createOperationNode(operators_.top());
        operators_.pop();
    }

    if (operands_.empty()) {
        throw ParseException(EMPTY_EXPRESSION);
    }
    if (operands_.size() > 1) {
        throw ParseException(UNUSED_OPERANDS);
    }

    return std::move(operands_.top());
}

void Parser::processBinaryToken(char token) {
    while (!operators_.empty() && precedence(operators_.top()) >= precedence(token)) {
        createOperationNode(operators_.top());
        operators_.pop();
    }
    operators_.push(token);
}