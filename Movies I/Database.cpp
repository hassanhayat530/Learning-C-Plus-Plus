#include <sstream>
#include <fstream>
#include <set>
#include <cctype>
#include <string>
#include "Movie.h"
#include "Database.h"

void Database::import(const std::string& filename, database_t& db) {
	std::fstream stream;
	stream.open(filename);

	if (!stream.good()) {
		throw Exception{ INPUT_ERROR_CODE, "Unable to open input file <" + filename + ">" };
	}

	import(stream, db);

	stream.close();
}

void Database::import(std::istream& stream, database_t& db) {
	std::string line;
	int lineNum = 1;

	while (std::getline(stream, line)) {
		if (!line.empty()) {
			++lineNum;
			parseLine(db, line, lineNum);
		}
	}
}


void Database::parseLine(database_t& db, std::string line, int lineNum) {
	std::istringstream lineStream(line);

	std::string name = parseStringField(lineStream, NAME_STRING, lineNum);
	unsigned short year = parseNumericField(lineStream, YEAR_STRING, YEAR_MIN, YEAR_MAX, lineNum);
	std::string genre = parseStringField(lineStream, GENRE_STRING, lineNum);
	unsigned short rating = parseNumericField(lineStream, RATING_STRING, RATING_MIN, RATING_MAX, lineNum);
	std::set<std::string> actorNames = parseActorField(lineStream, lineNum);

	db.emplace_back(Movie{ std::move(name), year, std::move(genre), rating, std::move(actorNames)});
}

std::string Database::parseStringField(std::istringstream& lineStream, const std::string& fieldName, int lineNum) {
	std::string value;

	if (!std::getline(lineStream, value, FIELD_DELIMITER)) {
		throwEmptyFieldError(fieldName, lineNum);
	}

	handleEmptyStringError(value, fieldName, lineNum);

	return value;
}

unsigned short Database::parseNumericField(std::istringstream& lineStream, const std::string& fieldName, unsigned short min, unsigned short max, int lineNum) {
	std::string valueString;

	if (!std::getline(lineStream, valueString, FIELD_DELIMITER)) {
		throwEmptyFieldError(fieldName, lineNum);
	}

	unsigned short value = parseInt(valueString, fieldName, lineNum);

	if (value < min || value > max) {
		throw Exception{ PARSING_ERROR_CODE, "Integer <" + valueString + "> out of range <" + std::to_string(min) + ", " + std::to_string(max) + "> in field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
	}

	return value;
}

std::set<std::string> Database::parseActorField(std::istringstream& lineStream, int lineNum) {
	std::set<std::string> actorNames;
	std::string actorList;

	if (std::getline(lineStream, actorList, FIELD_DELIMITER) && !actorList.empty()) {
		std::istringstream actorStream(actorList);
		std::string actor;

		while (std::getline(actorStream, actor, ACTOR_DELIMITER)) {

			if (!actor.empty()) {
				actorNames.insert(actor);
			}
		}
	}

	return actorNames;
}

void Database::throwEmptyFieldError(const std::string& fieldName, int lineNum) {
	throw Exception{ PARSING_ERROR_CODE, "Missing field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
}


void Database::handleEmptyStringError(const std::string& entry, const std::string& fieldName, int lineNum) {
	if (entry.empty()) {
		throw Exception{ PARSING_ERROR_CODE, "Empty string in field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
	}
}

unsigned short Database::parseInt(const std::string& entry, const std::string& fieldName, int lineNum) {
	try {
		size_t pos;
		unsigned short value = std::stoi(entry, &pos);

		if (pos != entry.length()) {
			throw Exception{ PARSING_ERROR_CODE, "Malformed integer <" + entry + "> in field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
		}

		return value;
	}

	catch (const std::invalid_argument&) {
		throw Exception{ PARSING_ERROR_CODE, "Invalid integer <" + entry + "> in field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
	}

	catch (const std::out_of_range&) {
		throw Exception{ PARSING_ERROR_CODE, "Overflow integer <" + entry + "> in field <" + fieldName + "> on line <" + std::to_string(lineNum) + ">" };
	}
}