#ifndef QUERIES_H
#define QUERIES_H

#include <iostream>
#include <vector>
#include <typeinfo>
#include "Storage.h"
#include "Title.h"

void db_query_1(const database_t& db, std::ostream& stream = std::cout);

void db_query_2(const database_t& db, std::ostream& stream = std::cout);

void db_query_3(
	const database_t& db,
	Type type, int begin, int end,
	std::ostream& stream
);

void db_query_4(
	const database_t& db,
	int seasons, int episodes,
	std::ostream& stream
);

void db_query_5(
	const database_t& db,
	int length,
	std::ostream& stream
);

#endif

