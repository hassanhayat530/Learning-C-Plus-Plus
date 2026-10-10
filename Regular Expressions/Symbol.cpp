#include "Symbol.h"

Symbol::Symbol(char symbol, size_t index) : symbol_(symbol), index_(index) {}

const char Symbol::symbol() const {
	return symbol_;
}

void Symbol::print(std::ostream& stream) const {
	stream << symbol_ << index_;
}

bool operator<(const Symbol& left, const Symbol& right) {
	return left.index_ < right.index_;
}