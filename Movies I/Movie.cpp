#include <iostream>
#include <string>
#include <set>
#include "Movie.h"
#include "Database.h"

Movie::Movie(const std::string& name, int year, const std::string& genre, int rating, const std::set<std::string>& actors) :
	name_(name), year_(year), genre_(genre), rating_(rating), actors_(actors) {
}

Movie::Movie(std::string&& name, int year, std::string&& genre, int rating, std::set<std::string>&& actors) :
	name_(std::move(name)), year_(year), genre_(std::move(genre)), rating_(rating), actors_(std::move(actors)) {
}

void Movie::print_json(std::ostream& stream) const {
	stream << "{ " << NAME_STRING << ": \"" << name_ << "\", " << YEAR_STRING << ": " << year_ << ", " << GENRE_STRING << ": \"" << genre_ << "\", " << RATING_STRING << ": " << rating_;

	if (actors_.size() == 0) {
		stream << " }";
	}

	else {
		stream << ", " << ACTORS_STRING << ": [ ";
		for (auto it = actors_.begin(); it != actors_.end();) {
			stream << "\"" << *it << "\"";
			if (++it != actors_.end()) stream << ", ";
		}
		stream << " ] }";
	}
}