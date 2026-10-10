#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <memory>
#include <sstream>
#include <fstream>
#include "Neighbors.h"
#include "Symbol.h"
#include "Result.h"

enum class Type {
	Number,
	Operation,
};

constexpr char CONCATENATION_OPERATOR = '.';
constexpr char ALTERNATION_OPERATOR = '+';
constexpr char ITERATION_OPERATOR = '*';
constexpr char SLASH_CHAR = '\\';
constexpr char SPACE = ' ';
constexpr int NULL_INDEX = 0;

enum class Precedence {
	NULL_ = 0,
	LOW = 1,
	MID = 2,
	HIGH = 3
};

class Node {
public:
	virtual ~Node() = default;
	virtual Result evaluate() const = 0;
};

class SymbolNode final : public Node {
private:
	Symbol symbol_;

public:
	SymbolNode(Symbol symbol);
	Result evaluate() const override;
};

class EmptyStringNode : public Node {
public:
	Result evaluate() const override;
};

class EmptyLanguageNode : public Node {
public:
	Result evaluate() const override;
};

class OperationNode : public Node {
protected:
	virtual Precedence precedence() const = 0;
public:
	virtual ~OperationNode() = default;
};

class UnaryOperationNode : public OperationNode {
protected:
	std::unique_ptr<Node> child_;
public:
	UnaryOperationNode(std::unique_ptr<Node> child);
	virtual ~UnaryOperationNode() = default;
};

class IterationNode final : public UnaryOperationNode {
public:
	using UnaryOperationNode::UnaryOperationNode;
	Result evaluate() const override;
	std::set<Neighbors> computeNeighbors(Result& r) const;
protected:
	Precedence precedence() const override;
};

class BinaryOperationNode : public OperationNode {
protected:
	std::unique_ptr<Node> left_;
	std::unique_ptr<Node> right_;

public:
	BinaryOperationNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
	virtual ~BinaryOperationNode() = default;
};

class ConcatenationNode final : public BinaryOperationNode {
public:
	using BinaryOperationNode::BinaryOperationNode;
	Result evaluate() const override;
	std::set<Symbol> computeStarting(const Result& r, const Result& s) const;
	std::set<Neighbors> computeNeighbors(const Result& r, const Result& s) const;
	std::set<Symbol> computeEnding(const Result& r, const Result& s) const;
protected:
	Precedence precedence() const override;
};

class AlternationNode final : public BinaryOperationNode {
public:
	using BinaryOperationNode::BinaryOperationNode;
	Result evaluate() const override;
protected:
	Precedence precedence() const override;
};

class Expression {
private:
	std::unique_ptr <Node> root_;

public:
	Expression(std::string_view input);
	Result evaluate() const;
	~Expression() = default;
};

std::set<Neighbors> neighborsUnion(const std::set<Neighbors>& set1, const std::set<Neighbors>& set2);

std::set<Symbol> symbolUnion(const std::set<Symbol>& set1, const std::set<Symbol>& set2);

#endif