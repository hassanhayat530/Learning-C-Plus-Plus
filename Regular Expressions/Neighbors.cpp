#include "Neighbors.h"
#include "Parser.h"

Neighbors::Neighbors(Symbol neighbor1, Symbol neighbor2) : neighbor1_(neighbor1), neighbor2_(neighbor2) {}

void Neighbors::print(std::ostream& stream) const {
	stream << LEFT_PARENTHESIS;
	neighbor1_.print(stream);
	stream << COMMA << SPACE;
	neighbor2_.print(stream);
	stream << RIGHT_PARENTHESIS;
}

bool operator<(const Neighbors& left, const Neighbors& right) {
	if (left.neighbor1_ < right.neighbor1_) {
		return true;
	}

	if (right.neighbor1_ < left.neighbor1_) {
		return false;
	}

	if (left.neighbor2_ < right.neighbor2_) {
		return true;
	}

	return false;
}