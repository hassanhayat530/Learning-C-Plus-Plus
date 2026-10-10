#ifndef ACTOR_H
#define ACTOR_H

#include <string>

class Actor {
private:
	std::string name_;
	std::string surname_;
	int year_;
public:
	Actor(const std::string& name, const std::string& surname, int year);
	Actor(std::string&& name, std::string&& surname, int year);

	std::string name();
	std::string surname();
	int year_();
};

#endif // !ACTOR_H
