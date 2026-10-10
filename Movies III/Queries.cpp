#include <iostream>
#include <vector>
#include <format>
#include <tuple>
#include "Database.h"
#include "Queries.h"


size_t std::hash<Actor>::operator()(const Actor& actor) const noexcept {
	return std::hash<std::string>{}(actor.surname());
}

void db_index_1(const database_t& database, index_titles_by_names_t& index) {
	for (const auto& title : database) {
		index.emplace(std::make_pair(title->name(), title));
	}
}

void db_index_2(const database_t& database, index_actors_by_years_t& index) {
	for (const auto& title : database) {
		const std::set<Actor>& actors = title->actors();

		for (const auto& actor : actors) {
			int birthYear = actor.year();
			std::set<Actor>& actorSet = index[birthYear];

			actorSet.insert(actor);
		}
	}
}

void db_index_3(const database_t& database, index_titles_by_years_t& index) {
	for (const auto& title : database) {
		index.emplace(std::make_pair(title->year(), title));
	}
}

void db_index_4(const database_t& database, index_titles_by_actors_t& index) {
	for (const auto& title : database) {
		const std::set<Actor>& actors = title->actors();

		for (const Actor& actor : actors) {
			index.emplace(std::make_pair(actor, title));
		}
	}
}

void db_index_5(const database_t& database, index_cast_by_genres_t& index) {
	for (const auto& title : database) {
		const std::set<Actor>& actors = title->actors();

		for (const Actor& actor : actors) {
			index.emplace(std::make_pair(
				std::make_tuple(title->genre(), title->year()),
				std::make_tuple(actor.name(), actor.surname(), title))
			);
		}
	}
}

bool Comparator_Q10_Actors::operator()(const Actor& actor1, const Actor& actor2) const {
	int year1 = actor1.year();
	int year2 = actor2.year();

	return std::tie(year1, actor1.name(), actor1.surname()) < std::tie(year2, actor2.name(), actor2.surname());
}

bool operator==(const Actor& actor1, const Actor& actor2) {
	int year1 = actor1.year();
	int year2 = actor2.year();

	return std::tie(actor1.name(), actor1.surname(), year1) == std::tie(actor2.name(), actor2.surname(), year2);
}

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
		if (title->type() == type && title->year() >= begin && title->year() < end) {
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
		if (title->type() == Type::SERIES) {
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

void db_query_6(const index_titles_by_names_t& index, std::string_view name, std::ostream& stream) {
	std::string nameString(name);

	auto find = index.find(nameString);

	if (find != index.end()) {
		find->second->print_json(stream);
	}
	else {
		stream << "Not found!";
	}
	stream << std::endl;
}

void db_query_7(const index_actors_by_years_t& index, int begin, int end, std::ostream& stream) {
	size_t count = 0;

	for (auto it = index.lower_bound(begin); it != index.lower_bound(end); ++it) {
		count += it->second.size();
	}

	stream << count << (count == 1 ? " actor" : " actors") << std::endl;
}

void print_title_and_year(const std::shared_ptr<Title>& title, std::ostream& stream) {
	stream << std::format("{{ {}: \"{}\", {}: {} }}", NAME_STRING, title->name(), YEAR_STRING, title->year()) << std::endl;
}

void db_query_8(const index_titles_by_years_t& index, int year, std::ostream& stream) {
	auto range = index.equal_range(year);

	if (range.first == range.second) {
		stream << "Not found!" << std::endl;
		return;
	}

	for (auto it = range.first; it != range.second; ++it) {
		const auto& title = it->second;

		print_title_and_year(title, stream);
	}
}

void db_query_9(const index_titles_by_years_t& index, int begin, int end, std::ostream& stream) {
	auto startIt = index.lower_bound(begin);
	auto endIt = index.lower_bound(end);

	if (startIt == endIt) {
		stream << "Not found!" << std::endl;
		return;
	}

	for (auto it = startIt; it != endIt; ++it) {
		const auto& title = it->second;

		print_title_and_year(title, stream);
	}
}

void db_query_10(const database_t& database, int begin, int end, std::ostream& stream) {
	std::multiset<Actor, Comparator_Q10_Actors> actorMultiset;

	for (const auto& title : database) {
		const std::set<Actor>& actors = title->actors();

		for (const Actor& actor : actors) {
			int birthYear = actor.year();

			if (birthYear >= begin && birthYear < end) {
				actorMultiset.insert(actor);
			}
		}
	}

	if (actorMultiset.empty()) {
		stream << "Not found!" << std::endl;
		return;
	}

	for (const Actor& actor : actorMultiset) {
		stream << actor << std::endl;
	}
}

bool predicate_Q11_movies(const Title* title) {
	if (title->type() != Type::MOVIE) {
		return false;
	}

	if (title->genre() == "comedy") {
		return false;
	}

	if (title->actors().size() < 3) {
		return false;
	}

	return true;
}

bool Predicate_Q11_Titles::operator()(const Title* title) const {
	if (title->rating() < 80) {
		return false;
	}

	Actor tatiana_vilhelmova("Tatiana", "Vilhelmova", 1978);

	const std::set<Actor>& actors = title->actors();

	return actors.find(tatiana_vilhelmova) != actors.end();
}

void db_query_11(const database_t& database, const std::function<bool(const Title*)>& predicate, std::ostream& stream) {
	bool found_any = false;

	for (const auto& title : database) {
		if (predicate(title.get())) {
			title->print_json(stream);
			stream << std::endl;
			found_any = true;
		}
	}

	if (!found_any) {
		stream << "Not found!" << std::endl;
	}
}

std::vector<Title*> db_query_12(const index_titles_by_actors_t& index, std::string_view surname) {
	std::vector<Title*> container;

	for (auto&& [key, value] : index) {
		if (key.surname() == surname) {
			container.push_back(value.get());
		}
	}

	return container;
}

std::vector<std::string> db_query_13(const index_cast_by_genres_t& index, std::string_view genre, int year) {
	std::vector<std::string> container;

	std::string genreString(genre);
	auto search_key = std::make_tuple(genreString, year);

	auto [beginIt, endIt] = index.equal_range(search_key);

	for (auto it = beginIt; it != endIt; ++it) {
		const auto& [first_name, last_name, title] = it->second;

		const std::string& title_name = title->name();

		container.emplace_back(
			std::format("{{ {}: \"{}\", {}: \"{}\", {}: \"{}\" }}",
				"name", first_name, "surname", last_name, "title", title_name)
		);;
	}

	return container;
}
