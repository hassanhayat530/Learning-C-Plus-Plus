#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <iostream>
#include <set>
#include "Symbol.h"
#include "Neighbors.h"

constexpr char EMPTY_STRING[] = "\\epsilon";
constexpr char EMPTY_SET[] = "\\emptyset";

class Result {
public:
	std::set<Symbol> starting;
	std::set<Neighbors> neighbors;
	std::set<Symbol> ending;
	bool epsilon;
};

#endif // !RESULT_H
