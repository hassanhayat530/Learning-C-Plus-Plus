#include <iostream>
#include <string>
#include <set>
#include <format>
#include "Title.h"
#include "Database.h"

Title::Title(const std::string& name, int year, const std::string& genre, int rating, const std::set<Actor>& actors) :
	name_(name), year_(year), genre_(genre), rating_(rating), actors_(actors) {
}

Title::Title(std::string&& name, int year, std::string&& genre, int rating, std::set<Actor>&& actors) :
	name_(std::move(name)), year_(year), genre_(std::move(genre)), rating_(rating), actors_(std::move(actors)) {
}

void Title::print_json_general(std::ostream& stream) const {
	stream << std::format("{}: \"{}\", {}: {}, {}: \"{}\", {}: {}",
		NAME_STRING, name_, YEAR_STRING, year_, GENRE_STRING, genre_, RATING_STRING, rating_);

	if (actors_.size() != 0) {
		stream << std::format(", {}: [ ", ACTORS_STRING);
		for (auto it = actors_.begin(); it != actors_.end();) {
			stream << *it;
			if (++it != actors_.end()) stream << ", ";
		}
		stream << " ]";
	}
}

std::ostream& operator<<(std::ostream& stream, const Title& title) {
	title.print_json(stream);
	return stream;
}

Movie::Movie(const std::string& name, int year, const std::string& genre, int rating, const std::set<Actor>& actors, std::optional<int> length) :
	Title(name, year, genre, rating, actors), length_(length) {
}

Movie::Movie(std::string&& name, int year, std::string&& genre, int rating, std::set<Actor>&& actors, std::optional<int> length) :
	Title(std::move(name), year, std::move(genre), rating, std::move(actors)), length_(length) {
}

Type Movie::get_type() const {
	return Type::MOVIE;
}

void Movie::print_json(std::ostream& stream) const {
	stream << std::format("{{ {}: \"{}\", ", TYPE, MOVIE);
	print_json_general(stream);

	if (length_.has_value()) {
		stream << std::format(", {}: {} }}", LENGTH_STRING, length_.value());
	}
	else {
		stream << " }";
	}
}

Series::Series(const std::string& name, int year, const std::string& genre, int rating, const std::set<Actor>& actors, int seasons, int episodes) :
	Title(name, year, genre, rating, actors), seasons_(seasons), episodes_(episodes) {
}

Series::Series(std::string&& name, int year, std::string&& genre, int rating, std::set<Actor>&& actors, int seasons, int episodes) :
	Title(std::move(name), year, std::move(genre), rating, std::move(actors)), seasons_(seasons), episodes_(episodes) {
}

Type Series::get_type() const {
	return Type::SERIES;
}

void Series::print_json(std::ostream& stream) const {
	stream << std::format("{{ {}: \"{}\", ", TYPE, SERIES);
	print_json_general(stream);
	stream << std::format(", {}: {}, {}: {} }}", SEASONS_STRING, seasons(), EPISODES_STRING, episodes());
}