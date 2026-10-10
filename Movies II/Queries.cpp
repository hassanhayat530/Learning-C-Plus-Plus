#include <iostream>
#include <vector>
#include <format>
#include "Title.h"
#include "Database.h"
#include "Queries.h"

void db_query_1(const database_t& db, std::ostream& stream) {
	for (auto&& title : db) {
		title->print_json(stream);
		stream << std::endl;
	}
}

void db_query_2(const database_t& db, std::ostream& stream) {
	Actor actor1{ "Ivan", "Trojan", 1964 };
	Actor actor2{ "Tereza", "Voriskova", 1989 };

	for (auto&& movie : db) {
		if (movie->genre() == "comedy" && movie->year() < 2010) {
			const std::set<Actor>& actors = movie->actors();
			if (actors.find(actor1) != actors.end() ||
				actors.find(actor2) != actors.end()) {
				stream << movie->name() << std::endl;
			}
		}
	}
}

void db_query_3(
	const database_t& db,
	Type type, int begin, int end,
	std::ostream& stream
) {
	for (auto&& title : db) {
		if (title->get_type() == type && title->year() >= begin && title->year() < end) {
			std::string type_str = (type == Type::MOVIE) ? MOVIE : SERIES;
			stream << std::format("{{ {}: \"{}\", {}: \"{}\", {}: {} }}",
				TYPE, type_str, NAME_STRING, title->name(), YEAR_STRING, title->year()) << std::endl;
		}
	}
}

void db_query_4(
	const database_t& db,
	int seasons, int episodes,
	std::ostream& stream
) {
	for (auto&& title : db) {
		if (title->get_type() == Type::SERIES) {
			auto series = std::static_pointer_cast<Series>(title);

			if (series->seasons() >= seasons || series->episodes() >= episodes) {
				stream << std::format("{{ {}: \"{}\", {}: {}, {}: {} }}",
					NAME_STRING, series->name(), SEASONS_STRING, series->seasons(),
					EPISODES_STRING, series->episodes()) << std::endl;
			}
		}
	}
}

void db_query_5(
	const database_t& db,
	int length,
	std::ostream& stream
) {
	for (auto&& title : db) {
		auto movie = std::dynamic_pointer_cast<Movie>(title);

		if (movie) {
			auto movie_length = movie->length();
			if (movie_length.has_value() && movie_length.value() >= length) {
				stream << std::format("{{ {}: \"{}\", {}: {} }}",
					NAME_STRING, movie->name(), LENGTH_STRING, movie_length.value()) << std::endl;
			}
		}
	}
}