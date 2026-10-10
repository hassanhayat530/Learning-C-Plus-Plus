#ifndef QUERIES_H
#define QUERIES_H

#include <iostream>
#include <vector>
#include <typeinfo>
#include <functional>
#include <string_view>
#include <map>
#include <unordered_map>
#include "Storage.h"

constexpr char TYPE[] = "type";
constexpr char MOVIE[] = "MOVIE";
constexpr char SERIES[] = "SERIES";
constexpr char NAME_STRING[] = "name";
constexpr char YEAR_STRING[] = "year";
constexpr char GENRE_STRING[] = "genre";
constexpr char RATING_STRING[] = "rating";
constexpr char ACTORS_STRING[] = "actors";
constexpr char LENGTH_STRING[] = "length";
constexpr char SEASONS_STRING[] = "seasons";
constexpr char EPISODES_STRING[] = "episodes";


using index_titles_by_names_t = std::map<std::string, std::shared_ptr<Title>>;
using index_actors_by_years_t = std::map<int, std::set<Actor>>;
using index_titles_by_years_t = std::multimap<int, std::shared_ptr<Title>>;
using index_titles_by_actors_t = std::unordered_multimap<Actor, std::shared_ptr<Title>>;
using index_cast_by_genres_t = std::multimap<std::tuple<std::string, int>, std::tuple<std::string, std::string, std::shared_ptr<Title>>>;

// Helper Functions
void print_title_and_year(const std::shared_ptr <Title>& title, std::ostream& stream = std::cout);

// Hash Functor Specialization
template<>
struct std::hash<Actor> {
	size_t operator()(const Actor& actor) const noexcept;
};

bool operator==(const Actor& actor1, const Actor& actor2);

// Index functions
void db_index_1(
	const database_t& database,
	index_titles_by_names_t& index
);

void db_index_2(
	const database_t& database,
	index_actors_by_years_t& index
);

void db_index_3(
	const database_t& database,
	index_titles_by_years_t& index
);

void db_index_4(
	const database_t& database,
	index_titles_by_actors_t& index
);

void db_index_5(
	const database_t& database,
	index_cast_by_genres_t& index
);

// Queries
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

void db_query_6(
	const index_titles_by_names_t& index,
	std::string_view name,
	std::ostream& stream = std::cout
);

void db_query_7(
	const index_actors_by_years_t& index,
	int begin, int end,
	std::ostream& stream = std::cout
);

void db_query_8(
	const index_titles_by_years_t& index,
	int year,
	std::ostream& stream = std::cout
);

void db_query_9(
	const index_titles_by_years_t& index,
	int begin, int end,
	std::ostream& stream = std::cout
);

class Comparator_Q10_Actors {
public:
	bool operator()(const Actor& actor1, const Actor& actor2) const;
};

void db_query_10(
	const database_t& database,
	int begin, int end,
	std::ostream& stream = std::cout
);

bool predicate_Q11_movies(const Title* title);

class Predicate_Q11_Titles {
public:
	bool operator()(const Title* title) const;
};


void db_query_11(
	const database_t& database,
	const std::function<bool(const Title*)>&
	predicate,
	std::ostream& stream = std::cout
);

std::vector<Title*> db_query_12(
	const index_titles_by_actors_t& index,
	std::string_view surname
);

std::vector<std::string> db_query_13(
	const index_cast_by_genres_t& index,
	std::string_view genre,
	int year
);

#endif

