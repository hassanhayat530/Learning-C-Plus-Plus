#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include "Symbol.h"

constexpr char COMMA = ',';

class Neighbors {
private:
	Symbol neighbor1_;
	Symbol neighbor2_;

public:
	Neighbors(Symbol neighbor1, Symbol neighbor2);
	void print(std::ostream& stream = std::cout) const;

	friend bool operator<(const Neighbors& left, const Neighbors& right);
};
#endif // !NEIGHBORS_H

