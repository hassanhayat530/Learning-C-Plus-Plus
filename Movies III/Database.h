#ifndef DATABASE_H
#define DATABASE_H

#include <sstream>
#include "Storage.h"

constexpr char NAME_STRING[] = "name";
constexpr char SURNAME_STRING[] = "surname";
constexpr char YEAR_STRING[] = "year";
constexpr char GENRE_STRING[] = "genre";
constexpr char RATING_STRING[] = "rating";
constexpr char ACTORS_STRING[] = "actors";
constexpr char LENGTH_STRING[] = "length";
constexpr char SEASONS_STRING[] = "seasons";
constexpr char EPISODES_STRING[] = "episodes";

constexpr char FIELD_DELIMITER = ';';
constexpr char ACTOR_DELIMITER = ',';
constexpr char SPACE_DELIMITER = ' ';

constexpr int YEAR_MIN = 1900;
constexpr int YEAR_MAX = 2100;
constexpr int RATING_MIN = 0;
constexpr int RATING_MAX = 100;
constexpr int ACTOR_YEAR_MIN = 1850;
constexpr int ACTOR_YEAR_MAX = 2100;
constexpr int LENGTH_MIN = 0;
constexpr int LENGTH_MAX = 300;
constexpr int SEASONS_MIN = 0;
constexpr int SEASONS_MAX = 100;
constexpr int EPISODES_MIN = 0;
constexpr int EPISODES_MAX = 10000;

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
	static void parseLine(database_t& db, std::string line, size_t lineNum);
	static std::string parseSelectorField(std::istringstream& lineStream, size_t lineNum);
	static std::string parseStringField(std::istringstream& lineStream, const std::string& fieldName, size_t lineNum);
	static std::optional<int> parseNumericField(
		std::istringstream& lineStream,
		const std::string& fieldName,
		int min,
		int max,
		size_t lineNum,
		bool required = true
	);
	static std::set<Actor> parseActorField(std::istringstream& lineStream, size_t lineNum);
	static int parseNumericFieldFromString(const std::string& valueString, const std::string& fieldName, int min, int max, size_t lineNum);
	static int parseInt(const std::string& entry, const std::string& fieldName, size_t lineNum);
	static void throwEmptyFieldError(const std::string& fieldName, size_t lineNum);
	static void handleEmptyStringError(const std::string& entry, const std::string& fieldName, size_t lineNum);
};

#endif