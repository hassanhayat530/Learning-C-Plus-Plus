#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>

class Actor {
private:
	std::string name_;
	std::string surname_;
	int year_;

public:
	Actor() = default;

	Actor(
		const std::string& name,
		const std::string& surname,
		int year
	);
	Actor(
		std::string&& name,
		std::string&& surname,
		int year
	);

	const std::string& name() const;
	const std::string& surname() const;
	int year() const;

	void print_json(std::ostream& stream = std::cout) const;


	friend std::ostream& operator<<(std::ostream& stream, const Actor& actor);
	friend std::istream& operator>>(std::istream& stream, Actor& actor);
};

inline const std::string& Actor::name() const {
	return name_;
}

inline const std::string& Actor::surname() const {
	return surname_;
}

inline int Actor::year() const {
	return year_;
}

std::ostream& operator<<(std::ostream& stream, const Actor& actor);
std::istream& operator>>(std::istream& stream, Actor& actor);
bool operator<(const Actor& actor1, const Actor& actor2);

#endif

