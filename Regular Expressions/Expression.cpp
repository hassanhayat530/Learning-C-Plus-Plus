#include <iostream>
#include <algorithm>
#include "Expression.h"
#include "Parser.h"

SymbolNode::SymbolNode(Symbol symbol) : symbol_(symbol) {}

Result SymbolNode::evaluate() const {
    Result result;
    result.epsilon = false;

    result.starting.insert(symbol_);
    result.ending.insert(symbol_);

    return result;
}

BinaryOperationNode::BinaryOperationNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right) : left_(std::move(left)), right_(std::move(right)) {}

UnaryOperationNode::UnaryOperationNode(std::unique_ptr<Node> child) : child_(std::move(child)) {}

Result ConcatenationNode::evaluate() const {
    Result result;
    Result leftResult = left_->evaluate();
    Result rightResult = right_->evaluate();

    result.starting = computeStarting(leftResult, rightResult);
    result.neighbors = computeNeighbors(leftResult, rightResult);
    result.ending = computeEnding(leftResult, rightResult);

    result.epsilon = leftResult.epsilon && rightResult.epsilon;

    return result;
}

std::set<Symbol> ConcatenationNode::computeStarting(const Result& r, const Result& s) const {
    if (!r.epsilon) {
        return r.starting;
    }

    else {
        std::set<Symbol> resultStarting = symbolUnion(r.starting, s.starting);
        return resultStarting;
    }
}

std::set<Neighbors> ConcatenationNode::computeNeighbors(const Result& r, const Result& s) const {
    std::set<Neighbors> resultNeighbors = neighborsUnion(r.neighbors, s.neighbors);

    for (const auto& x : r.ending) {
        for (const auto& y : s.starting) {
            resultNeighbors.emplace(x, y);
        }
    }

    return resultNeighbors;
}

std::set<Symbol> ConcatenationNode::computeEnding(const Result& r, const Result& s) const {
    if (!s.epsilon) {
        return s.ending;
    }

    else {
        std::set<Symbol> resultStarting = symbolUnion(s.ending, r.ending);
        return resultStarting;
    }
}

Result EmptyStringNode::evaluate() const {
    Result result;
    result.epsilon = true;
    return result;
}

Result EmptyLanguageNode::evaluate() const {
    Result result;
    result.epsilon = false;
    return result;
}

Result AlternationNode::evaluate() const {
    Result result;
    Result leftResult = left_->evaluate();
    Result rightResult = right_->evaluate();

    result.starting = symbolUnion(leftResult.starting, rightResult.starting);
    result.neighbors = neighborsUnion(leftResult.neighbors, rightResult.neighbors);
    result.ending = symbolUnion(leftResult.ending, rightResult.ending);

    result.epsilon = leftResult.epsilon || rightResult.epsilon;

    return result;
}

Result IterationNode::evaluate() const {
    Result result;
    Result childResult = child_->evaluate();

    result.starting = childResult.starting;
    result.neighbors = computeNeighbors(childResult);
    result.ending = childResult.ending;

    result.epsilon = true;

    return result;
}

std::set<Neighbors> IterationNode::computeNeighbors(Result& r) const {
    std::set<Neighbors> resultNeighbors = r.neighbors;

    for (const auto& x : r.ending) {
        for (const auto& y : r.starting) {
            resultNeighbors.insert(Neighbors(x, y));
        }
    }

    return resultNeighbors;
}

Expression::Expression(std::string_view input) {
    root_ = Parser::parse(input);
}

Result Expression::evaluate() const {
    return root_->evaluate();
}

std::set<Neighbors> neighborsUnion(const std::set<Neighbors>& set1, const std::set<Neighbors>& set2) {
    std::set<Neighbors> result = set1;
    result.insert(set2.begin(), set2.end());
    return result;
}

std::set<Symbol> symbolUnion(const std::set<Symbol>& set1, const std::set<Symbol>& set2) {
    std::set<Symbol> result = set1;
    result.insert(set2.begin(), set2.end());
    return result;
}

Precedence IterationNode::precedence() const {
    return Precedence::HIGH;
}

Precedence ConcatenationNode::precedence() const {
    return Precedence::MID;
}

Precedence AlternationNode::precedence() const {
    return Precedence::LOW;
}