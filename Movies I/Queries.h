#pragma once
#ifndef QUERIES_H
#define QURIES_H

#include <iostream>
#include "Storage.h"

void db_query_1(const database_t& db, std::ostream& stream = std::cout);

void db_query_2(const database_t& db, std::ostream& stream = std::cout);

#endif

