#include "Expression.h"
#include "Parser.h"

NumberNode::NumberNode(int number) : number_(number) {}

OperationNode::OperationNode(Node* left, Node* right) : left_(left), right_(right) {}

Expression::Expression(Node* root) : root_(root) {}

Type NumberNode::get_type() const {
	return Type::NUMBER;
}

int NumberNode::evaluate() const {
	return number_;
}

void NumberNode::print_postfix(std::ostream& stream) const {
	stream << number_;
}

void NumberNode::print_infix(std::ostream& stream) const {
	stream << number_;
}

Type OperationNode::get_type() const {
	return Type::OPERATION;
}

OperationNode::~OperationNode() {
	if (left_ != nullptr) {
		delete left_;
		left_ = nullptr;
	}
	if (right_ != nullptr) {
		delete right_;
		right_ = nullptr;
	}
}

void OperationNode::print_postfix(std::ostream& stream) const {
	left_->print_postfix(stream);
	stream << SPACE;
	right_->print_postfix(stream);
	stream << SPACE;
	stream << get_operator();
}

void OperationNode::printSide(std::ostream& stream, Node* side, bool isRightChild) const {
	Precedence sidePrecedence = this->precedence();
	bool needsParentheses = false;

	if (side->get_type() == Type::OPERATION) {
		OperationNode* sideOperation = static_cast<OperationNode*>(side);
		Precedence childPrecedence = sideOperation->precedence();
		if (childPrecedence < sidePrecedence) {
			needsParentheses = true;
		}
		if (isRightChild && childPrecedence <= sidePrecedence) {
			needsParentheses = true;
		}
	}

	if (needsParentheses) {
		stream << LEFT_PARENTHESIS;
		side->print_infix(stream);
		stream << RIGHT_PARENTHESIS;
	}
	else {
		side->print_infix(stream);
	}
}

void OperationNode::print_infix(std::ostream& stream) const {
	printSide(stream, left_, false);
	stream << get_operator();
	printSide(stream, right_, true);
}

char AdditionNode::get_operator() const {
	return ADD_OPERATOR;
}

int AdditionNode::evaluate() const {
	return left_->evaluate() + right_->evaluate();
}

char SubtractionNode::get_operator() const {
	return SUB_OPERATOR;
}

int SubtractionNode::evaluate() const {
	return left_->evaluate() - right_->evaluate();
}

char MultiplicationNode::get_operator() const {
	return MUL_OPERATOR;
}

int MultiplicationNode::evaluate() const {
	return left_->evaluate() * right_->evaluate();
}

char DivisionNode::get_operator() const {
	return DIV_OPERATOR;
}

int DivisionNode::evaluate() const {
	int divisor = right_->evaluate();
	if (divisor == 0) {
		throw EvaluationException("Division by zero");
	}
	return left_->evaluate() / divisor;
}

Expression::~Expression() {
	delete root_;
}

int Expression::evaluate() const {
	if (root_ == nullptr) {
		return 0;
	}
	return root_->evaluate();
}

void Expression::print_postfix(std::ostream& stream) const {
	root_->print_postfix(stream);
}

void Expression::print_infix(std::ostream& stream) const {
	root_->print_infix(stream);
}

Expression::Expression(std::string_view input) {
	root_ = Parser::parse(input);
}