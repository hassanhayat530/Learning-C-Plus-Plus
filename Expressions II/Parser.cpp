#include "Parser.h"

Node* Parser::parse(std::string_view input) {
    try {
        Parser parser;
        return parser.createExpression(input);
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

Node* Parser::createExpression(std::string_view input) {
    try {
        if (input.empty()) {
            throw ParseException(EMPTY_EXPRESSION);
        }

        size_t numberStart = 0;
        bool parsingNumber = false;

        for (size_t i = 0; i < input.length(); ++i) {
            char token = input[i];

            checkUnknownToken(token);

            if (std::isdigit(token)) {
                if (!parsingNumber) {
                    numberStart = i;
                    parsingNumber = true;
                }
            }
            else {
                if (parsingNumber) {
                    pushNumberNode(input.substr(numberStart, i - numberStart));
                    parsingNumber = false;
                }

                if (token == LEFT_PARENTHESIS) {
                    processLeftParenthesisToken();
                }
                else if (token == RIGHT_PARENTHESIS) {
                    processRightParenthesisToken();
                }
                else if (isOperator(token)) {
                    processOperatorToken(token);
                }
            }
        }

        if (parsingNumber) {
            pushNumberNode(input.substr(numberStart));
        }

        while (!operators_.empty()) {
            if (operators_.top() == LEFT_PARENTHESIS) {
                cleanUp();
                throw ParseException(OPENING_PARANTHESIS_MSG);
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

        return operands_.top();
    }
    catch (const std::bad_alloc&) {
        cleanUp();
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
    catch (const Exception&) {
        cleanUp();
        throw;
    }
}

Precedence Parser::precedence(char op) const {
    switch (op) {
    case MUL_OPERATOR:
    case DIV_OPERATOR:
        return Precedence::HIGH;
    case ADD_OPERATOR:
    case SUB_OPERATOR:
        return Precedence::LOW;
    default:
        throw ParseException(UNKOWN_TOKEN);
    }
}

bool Parser::isOperator(char t) const {
    return (t == ADD_OPERATOR || t == SUB_OPERATOR || t == DIV_OPERATOR || t == MUL_OPERATOR);
}

void Parser::cleanUp() {
    while (!operands_.empty()) {
        delete operands_.top();
        operands_.pop();
    }
}

void Parser::checkUnknownToken(char token) {
    if (isOperator(token) || isdigit(token) || token == LEFT_PARENTHESIS || token == RIGHT_PARENTHESIS) {
        return;
    }
    else {
        throw ParseException(UNKOWN_TOKEN);
    }
}

int Parser::parseNumber(std::string_view number) {
    try {
        std::string numberStr(number);
        int value = std::stoi(numberStr);
        return value;
    }
    catch (const std::out_of_range&) {
        throw ParseException(OVERFLOW_NUMBER);
    }
}

void Parser::pushNumberNode(std::string_view number) {
    try {
        int numValue = parseNumber(number);
        Node* newNode = new NumberNode(numValue);
        try {
            operands_.push(newNode);
        }
        catch (const std::bad_alloc&) {
            delete newNode;
            throw MemoryException(UNAVAILABLE_MEMORY);
        }
    }
    catch (const std::bad_alloc&) {
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::createOperationNode(char o) {
    if (operands_.size() < 2) {
        throw ParseException(MISSING_OPERANDS);
    }

    Node* r = operands_.top();
    operands_.pop();
    Node* l = operands_.top();
    operands_.pop();

    try {
        Node* operation = nullptr;

        switch (o) {
        case ADD_OPERATOR:
            operation = new AdditionNode(l, r);
            break;
        case SUB_OPERATOR:
            operation = new SubtractionNode(l, r);
            break;
        case MUL_OPERATOR:
            operation = new MultiplicationNode(l, r);
            break;
        case DIV_OPERATOR:
            operation = new DivisionNode(l, r);
            break;
        }

        try {
            operands_.push(operation);
        }
        catch (const std::bad_alloc&) {
            delete operation;
            throw MemoryException(UNAVAILABLE_MEMORY);
        }
    }
    catch (const std::bad_alloc&) {
        delete l;
        delete r;
        throw MemoryException(UNAVAILABLE_MEMORY);
    }
}

void Parser::processLeftParenthesisToken() {
    operators_.push(LEFT_PARENTHESIS);
}

void Parser::processRightParenthesisToken() {
    while (!operators_.empty() && operators_.top() != LEFT_PARENTHESIS) {
        createOperationNode(operators_.top());
        operators_.pop();

    }
    if (operators_.empty()) {
        throw ParseException(CLOSING_PARENTHESIS_MSG);
    }
    operators_.pop();
}

void Parser::processOperatorToken(char token) {
    while (!operators_.empty() && operators_.top() != LEFT_PARENTHESIS &&
        precedence(operators_.top()) >= precedence(token)) {
        createOperationNode(operators_.top());
        operators_.pop();
    }
    operators_.push(token);
}