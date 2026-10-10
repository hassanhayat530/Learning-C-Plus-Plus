#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include "Actor.h"
#include "Database.h"

Actor::Actor(const std::string& name, const std::string& surname, int year) :
	name_(name), surname_(surname), year_(year) {
}

Actor::Actor(std::string&& name, std::string&& surname, int year) :
	name_(std::move(name)), surname_(std::move(surname)), year_(year) {
}

void Actor::print_json(std::ostream& stream) const {
	stream << std::format("{{ {}: \"{}\", {}: \"{}\", {}: {} }}", NAME_STRING, name_, SURNAME_STRING, surname_, YEAR_STRING, year_);
}

bool operator<(const Actor& actor1, const Actor& actor2) {
	return actor1.surname() != actor2.surname() ? actor1.surname() < actor2.surname() :
		actor1.name() != actor2.name() ? actor1.name() < actor2.name() :
		actor1.year() < actor2.year();
}


std::ostream& operator<<(std::ostream& stream, const Actor& actor) {
	actor.print_json(stream);
	return stream;
}

std::istream& operator>>(std::istream& stream, Actor& actor) {

    if (!(stream >> actor.name_)) {
        throw Exception{ PARSING_ERROR_CODE, "Missing attribute <name>" };
    }
    if (!(stream >> actor.surname_)) {
        throw Exception{ PARSING_ERROR_CODE, "Missing attribute <surname>" };
    }
    if (!(stream >> actor.year_)) {
        throw Exception{ PARSING_ERROR_CODE, "Missing, invalid, or overflow value in attribute <year>" };
    }

    if (actor.year_ < ACTOR_YEAR_MIN || actor.year_ > ACTOR_YEAR_MAX) {
        throw Exception{ PARSING_ERROR_CODE, std::format("Integer <{}> out of range <{}, {}> in attribute <year>",
            actor.year_, ACTOR_YEAR_MIN, ACTOR_YEAR_MAX) };
    }

    return stream;
}