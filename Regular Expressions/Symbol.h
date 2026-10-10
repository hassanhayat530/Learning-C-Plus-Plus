#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>

class Symbol {
private:
	char symbol_;
	int index_;

public:
	Symbol(char symbol, size_t index);
	const char symbol() const;
	void print(std::ostream& stream = std::cout) const;

	friend bool operator<(const Symbol& left, const Symbol& right);
};

#endif // !SYMBOL_H

