#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>
#include <set>

class Movie {
private:
	std::string name_;
	int year_;
	std::string genre_;
	int rating_;
	std::set<std::string> actors_;

public:
	Movie(const std::string& name, int year, const std::string& genre, int rating, const std::set<std::string>& actorNames);

	Movie(std::string&& name, int year, std::string&& genre, int rating, std::set<std::string>&& actors);

	inline const std::string& name() const;
	inline const int year() const;
	inline const std::string& genre() const;
	inline const int rating() const;
	inline const std::set<std::string>& actors() const;

	void print_json(std::ostream& stream = std::cout) const;
};

inline const std::string& Movie::name() const {
	return name_;
}

inline const int Movie::year() const {
	return year_;
}

inline const std::string& Movie::genre() const {
	return genre_;
}

inline const int Movie::rating() const {
	return rating_;
}

inline const std::set<std::string>& Movie::actors() const {
	return actors_;
}

#endif
