#include <iostream>
#include <vector>
#include "Movie.h"
#include "Queries.h"

void db_query_1(const database_t& db, std::ostream& stream) {
	for (auto&& movie : db) {
		movie.print_json(stream);
		stream << std::endl;
	}
}

void db_query_2(const database_t& db, std::ostream& stream) {
    for (auto&& movie : db) {
        if (movie.genre() == "comedy" && movie.year() < 2010) {
            const std::set<std::string>& actors = movie.actors();
            if (actors.find("Ivan Trojan") != actors.end() ||
                actors.find("Tereza Voriskova") != actors.end()) {
                stream << movie.name() << std::endl;
            }
        }
    }
}