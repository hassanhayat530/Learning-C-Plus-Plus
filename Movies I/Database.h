#ifndef DATABASE_H
#define DATABASE_H

#include <sstream>
#include "Storage.h"

constexpr char NAME_STRING[] = "name";
constexpr char YEAR_STRING[] = "year";
constexpr char GENRE_STRING[] = "genre";
constexpr char RATING_STRING[] = "rating";
constexpr char ACTORS_STRING[] = "actors";

constexpr char FIELD_DELIMITER = ';';
constexpr char ACTOR_DELIMITER = ',';

constexpr unsigned short YEAR_MIN = 1900;
constexpr unsigned short YEAR_MAX = 2100;
constexpr unsigned short RATING_MIN = 0;
constexpr unsigned short RATING_MAX = 100;

constexpr int INPUT_ERROR_CODE = 1;
constexpr int PARSING_ERROR_CODE = 2;

struct Exception {
	int code;
	std::string text;
};

class Database {
public:
	static void import(const std::string& filename, database_t& db);

	static void import(std::istream& stream, database_t& db);

private:
	static void parseLine(database_t& db, std::string line, int lineNum);
	static std::string parseStringField(std::istringstream& lineStream, const std::string& fieldName, int lineNum);
	static unsigned short parseNumericField(std::istringstream& lineStream, const std::string& fieldName, unsigned short min, unsigned short max, int lineNum);
	static std::set<std::string> parseActorField(std::istringstream& lineStream, int lineNum);
	static unsigned short parseInt(const std::string& entry, const std::string& fieldName, int lineNum);
	static void throwEmptyFieldError(const std::string& fieldName, int lineNum);
	static void handleEmptyStringError(const std::string& entry, const std::string& fieldName, int lineNum);
};

#endif
