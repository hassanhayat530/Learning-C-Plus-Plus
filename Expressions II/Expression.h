#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include "Exception.h"

constexpr char ADD_OPERATOR = '+';
constexpr char SUB_OPERATOR = '-';
constexpr char MUL_OPERATOR = '*';
constexpr char DIV_OPERATOR = '/';
constexpr char LEFT_PARENTHESIS = '(';
constexpr char RIGHT_PARENTHESIS = ')';
constexpr char SPACE = ' ';

enum class Type {
	NUMBER,
	OPERATION
};

enum class Precedence {
	LOW = 1,
	HIGH = 2
};

class Node {
public:
	virtual Type get_type() const = 0;
	virtual ~Node() = default;
	virtual int evaluate() const = 0;
	virtual void print_postfix(std::ostream& stream = std::cout) const = 0;
	virtual void print_infix(std::ostream& stream = std::cout) const = 0;
};

class NumberNode final : public Node {
private:
	int number_;
public:
	NumberNode(int number);
	Type get_type() const override;
	int evaluate() const override;
	void print_postfix(std::ostream& stream = std::cout) const override;
	void print_infix(std::ostream& stream = std::cout) const override;
};

class OperationNode : public Node {
protected:
	Node* left_;
	Node* right_;
	virtual char get_operator() const = 0;
	virtual Precedence precedence() const = 0;
public:
	OperationNode(Node* left, Node* right);
	Type get_type() const override;
	virtual ~OperationNode();
	void print_postfix(std::ostream& stream = std::cout) const override;
	void print_infix(std::ostream& stream = std::cout) const override;

private:
	void printSide(std::ostream& stream, Node* side, bool isRightChild) const;
};

class AdditionNode final : public OperationNode {
public:
	using OperationNode::OperationNode;
	int evaluate() const override;
protected:
	char get_operator() const override;
	inline Precedence precedence() const override {
		return Precedence::LOW;
	}
};

class SubtractionNode final : public OperationNode {
public:
	using OperationNode::OperationNode;
	int evaluate() const override;
protected:
	char get_operator() const override;
	inline Precedence precedence() const override {
		return Precedence::LOW;
	}
};

class MultiplicationNode final : public OperationNode {
public:
	using OperationNode::OperationNode;
	int evaluate() const override;
protected:
	char get_operator() const override;
	inline Precedence precedence() const override {
		return Precedence::HIGH;
	}
};

class DivisionNode final : public OperationNode {
public:
	using OperationNode::OperationNode;
	int evaluate() const override;
protected:
	char get_operator() const override;
	inline Precedence precedence() const override {
		return Precedence::HIGH;
	}
};

class Expression {
private:
	Node* root_;
public:
	Expression(std::string_view input);
	Expression(Node* root);
	~Expression();
	int evaluate() const;
	void print_postfix(std::ostream& stream = std::cout) const;
	void print_infix(std::ostream& stream = std::cout) const;
};

#endif // !EXPRESSION_H

