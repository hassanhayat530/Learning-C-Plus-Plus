#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>
#include <string>

constexpr char INPUT_ERROR[] = "Unable to open input file";
constexpr char OUTPUT_ERROR[] = "Unable to open output file";
constexpr char INVALID_WORD_ERROR[] = "Invalid word detected";
constexpr char INVALID_INT_ERROR[] = "Invalid integer number detected";
constexpr char INVALID_FLOAT_ERROR[] = "Invalid floating point number detected";

constexpr char LINES[] = "Lines";
constexpr char SENTENCES[] = "Sentences";
constexpr char WORDS[] = "Words";
constexpr char NUMBERS[] = "Numbers";
constexpr char LETTERS[] = "Letters";
constexpr char DIGITS[] = "Digits";
constexpr char SPACES[] = "Spaces";
constexpr char SYMBOLS[] = "Symbols";
constexpr char INTEGERS[] = "Integers";
constexpr char FLOATS[] = "Floats";

struct Statistics {
	size_t lines = 0;
	size_t sentences = 0;
	size_t words = 0;
	size_t numbers = 0;
	size_t letters = 0;
	size_t digits = 0;
	size_t spaces = 0;
	size_t symbols = 0;
	long integers = 0;
	double floats = 0;
};

class Counter {
public:
	static void process(const std::string& filename, Statistics* data);

	static void process(std::istream& stream, Statistics* data);

	static void print(const std::string& filename, const Statistics* data);

	static void print(std::ostream& stream, const Statistics* data);

private:
	static void processLine(std::string& currentLine, Statistics* data);
	static void processNumber(std::string& currentNumber, Statistics* data, bool& isFloat);
	static double parseFloat(const std::string& floatString);
	static int parseInteger(const std::string& integerString);

};

#endif // !COUNTER_H

