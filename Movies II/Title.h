#ifndef TITLE_H
#define TITLE_H

#include <iostream>
#include <string>
#include <set>
#include <memory>
#include <optional>
#include "Actor.h"

constexpr char TYPE[] = "type";
constexpr char MOVIE[] = "MOVIE";
constexpr char SERIES[] = "SERIES";

enum class Type {
	MOVIE,
	SERIES,
};

// -----------------------------------------------------------------------------

class Title {
private:
	std::string name_;
	int year_;
	std::string genre_;
	int rating_;
	std::set<Actor> actors_;

public:
	Title(const std::string& name, int year, const std::string& genre, int rating, const std::set<Actor>& actorNames);

	Title(std::string&& name, int year, std::string&& genre, int rating, std::set<Actor>&& actors);

	inline const std::string& name() const;
	inline const int year() const;
	inline const std::string& genre() const;
	inline const int rating() const;
	inline const std::set<Actor>& actors() const;

	void print_json_general(std::ostream& stream = std::cout) const;

	virtual Type get_type() const = 0;
	virtual void print_json(std::ostream& stream = std::cout) const = 0;
	virtual ~Title() = default;

	friend std::ostream& operator<<(std::ostream& stream, const Title& title);
};

// -----------------------------------------------------------------------------

inline const std::string& Title::name() const {
	return name_;
}

inline const int Title::year() const {
	return year_;
}

inline const std::string& Title::genre() const {
	return genre_;
}

inline const int Title::rating() const {
	return rating_;
}

inline const std::set<Actor>& Title::actors() const {
	return actors_;
}

// -----------------------------------------------------------------------------

class Movie : public Title {
protected:
	std::optional<int> length_;

public:
	Type get_type() const override;

	Movie(
		const std::string& name,
		int year,
		const std::string& genre,
		int rating,
		const std::set<Actor>& actors,
		std::optional<int> length = std::nullopt
	);
	Movie(
		std::string&& name,
		int year,
		std::string&& genre,
		int rating,
		std::set<Actor>&& actors,
		std::optional<int> length = std::nullopt
	);

	std::optional<int> length() const;
	void print_json(std::ostream& stream = std::cout) const override;
};

// -----------------------------------------------------------------------------

class Series : public Title {
protected:
	int seasons_;
	int episodes_;

public:
	Type get_type() const override;

	Series(
		const std::string& name,
		int year,
		const std::string& genre,
		int rating,
		const std::set<Actor>& actors,
		int seasons,
		int episodes
	);
	Series(
		std::string&& name,
		int year,
		std::string&& genre,
		int rating,
		std::set<Actor>&& actors,
		int seasons,
		int episodes
	);

	int seasons() const;
	int episodes() const;
	void print_json(std::ostream& stream = std::cout) const override;

};

// -----------------------------------------------------------------------------

inline std::optional<int> Movie::length() const {
	return length_;
}

inline int Series::seasons() const {
	return seasons_;
}

inline int Series::episodes() const {
	return episodes_;
}

// -----------------------------------------------------------------------------

#endif
