#include <sstream>
#include <fstream>
#include <set>
#include <cctype>
#include <string>
#include <format>
#include "Title.h"
#include "Database.h"

void Database::import(const std::string& filename, database_t& db) {
	std::fstream stream;
	stream.open(filename);

	if (!stream.good()) {
		throw Exception{ INPUT_ERROR_CODE, std::format("Unable to open input file <{}>", filename) };
	}

import(stream, db);

	stream.close();
}

void Database::import(std::istream& stream, database_t& db) {
	std::string line;
	size_t lineNum = 0;

	while (std::getline(stream, line)) {
		++lineNum;
		if (!line.empty()) {
			parseLine(db, line, lineNum);
		}
	}
}


void Database::parseLine(database_t& db, std::string line, size_t lineNum) {
	std::istringstream lineStream(line);

	std::string type = parseSelectorField(lineStream, lineNum);
	std::string name = parseStringField(lineStream, NAME_STRING, lineNum);

	int year = parseNumericField(lineStream, YEAR_STRING, YEAR_MIN, YEAR_MAX, lineNum, true).value();

	std::string genre = parseStringField(lineStream, GENRE_STRING, lineNum);

	int rating = parseNumericField(lineStream, RATING_STRING, RATING_MIN, RATING_MAX, lineNum, true).value();

	std::set<Actor> actors = parseActorField(lineStream, lineNum);

	if (type == MOVIE) {
		auto lengthOpt = parseNumericField(lineStream, LENGTH_STRING, LENGTH_MIN, LENGTH_MAX, lineNum, false);
        
        db.emplace_back(std::make_shared<Movie>(
            std::move(name), year, std::move(genre), rating, 
            std::move(actors), lengthOpt));
	}
	else if (type == SERIES) {
		int seasons = parseNumericField(lineStream, SEASONS_STRING, SEASONS_MIN, SEASONS_MAX, lineNum, true).value();

		int episodes = parseNumericField(lineStream, EPISODES_STRING, EPISODES_MIN, EPISODES_MAX, lineNum, true).value();

		db.emplace_back(std::make_shared<Series>(std::move(name), year, std::move(genre), rating, std::move(actors), seasons, episodes));
	}
}

std::string Database::parseSelectorField(std::istringstream& lineStream, size_t lineNum) {
	std::string selector;
	std::getline(lineStream, selector, FIELD_DELIMITER);

	if (selector != MOVIE && selector != SERIES) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Invalid type selector <{}> in field <type> on line <{}>", selector, lineNum) };
	}

	return selector;
}

std::string Database::parseStringField(std::istringstream& lineStream, const std::string& fieldName, size_t lineNum) {
	std::string value;

	if (!std::getline(lineStream, value, FIELD_DELIMITER)) {
		throwEmptyFieldError(fieldName, lineNum);
	}

	handleEmptyStringError(value, fieldName, lineNum);

	return value;
}

std::optional<int> Database::parseNumericField(
	std::istringstream& lineStream,
	const std::string& fieldName,
	int min,
	int max,
	size_t lineNum,
	bool required
) {
	std::string valueString;

	if (!std::getline(lineStream, valueString, FIELD_DELIMITER)) {
		if (required) {
			throwEmptyFieldError(fieldName, lineNum);
		}
		return std::nullopt;
	}

	if (valueString.empty()) {
		if (required) {
			throw Exception{ PARSING_ERROR_CODE, std::format("Empty string in field <{}> on line <{}>", fieldName, lineNum) };
		}
		return std::nullopt;
	}
	
	int value = parseInt(valueString, fieldName, lineNum);

	if (value < min || value > max) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Integer <{}> out of range <{}, {}> in field <{}> on line <{}>", valueString, min, max, fieldName, lineNum) };
	}

	return value;
}

std::set<Actor> Database::parseActorField(std::istringstream& lineStream, size_t lineNum) {
	std::set<Actor> actorNames;
	std::string actorList;

	if (std::getline(lineStream, actorList, FIELD_DELIMITER) && !actorList.empty()) {
		std::istringstream actorListStream(actorList);
		std::string actorString;

		while (std::getline(actorListStream, actorString, ACTOR_DELIMITER)) {
			if (!actorString.empty()) {
				std::istringstream actorStream(actorString);

				try {
					Actor actor;
					actorStream >> actor;

					if (!actorStream) {
						throw Exception{ PARSING_ERROR_CODE,
							std::format("Missing, invalid, or overflow value in attribute <year> in actor <{}> on line <{}>",
								actorString, lineNum) };
					}

					actorNames.insert(std::move(actor));
				}
				catch (const Exception& e) {
					throw Exception{ e.code,
						std::format("{} in actor <{}> on line <{}>", e.text, actorString, lineNum) };
				}
			}
		}
	}

	return actorNames;
}

int Database::parseNumericFieldFromString(const std::string& valueString, const std::string& fieldName, int min, int max, size_t lineNum) {
	unsigned short value = parseInt(valueString, fieldName, lineNum);

	if (value < min || value > max) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Integer <{}> out of range <{}, {}> in field <{}> on line <{}>", valueString, min, max, fieldName, lineNum) };
	}

	return value;
}

void Database::throwEmptyFieldError(const std::string& fieldName, size_t lineNum) {
	throw Exception{ PARSING_ERROR_CODE, std::format("Missing field <{}> on line <{}>", fieldName, lineNum) };
}


void Database::handleEmptyStringError(const std::string& entry, const std::string& fieldName, size_t lineNum) {
	if (entry.empty()) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Empty string in field <{}> on line <{}>", fieldName, lineNum) };
	}
}

int Database::parseInt(const std::string& entry, const std::string& fieldName, size_t lineNum) {
	try {
		size_t pos;
		int value = std::stoi(entry, &pos);

		if (pos != entry.length()) {
			throw Exception{ PARSING_ERROR_CODE, std::format("Malformed integer <{}> in field <{}> on line <{}>", entry, fieldName, lineNum) };
		}

		return value;
	}

	catch (const std::invalid_argument&) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Invalid integer <{}> in field <{}> on line <{}>", entry, fieldName, lineNum) };
	}

	catch (const std::out_of_range&) {
		throw Exception{ PARSING_ERROR_CODE, std::format("Overflow integer <{}> in field <{}> on line <{}>", entry, fieldName, lineNum) };
	}
}