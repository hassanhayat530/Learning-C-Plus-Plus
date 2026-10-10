#include "Counter.h"
#include <fstream>
#include <cctype>
#include <iomanip>

void Counter::process(const std::string& filename, Statistics* data) {
    std::ifstream stream(filename);

    if (!stream.is_open()) {
        throw INPUT_ERROR;
    }

    process(stream, data);
}

void Counter::process(std::istream& stream, Statistics* data) {
    std::string currentLine;

    while (std::getline(stream, currentLine)) {
        ++data->lines;
        processLine(currentLine, data);
    }
}

void Counter::processLine(std::string& currentLine, Statistics* data) {
    bool inWord = false;
    bool inNumber = false;
    bool isFloat = false;
    std::string currentNumber;

    for (size_t i = 0; i < currentLine.length(); ++i) {
        char currChar = currentLine[i];

        if (std::isalpha(currChar)) {
            if (inNumber) {
                if (isFloat) {
                    throw INVALID_FLOAT_ERROR;
                }
                else {
                    throw INVALID_INT_ERROR;
                }
            }
            if (!inWord) {
                ++data->words;
                inWord = true;
            }
            ++data->letters;
            inNumber = false;
        }

        else if (std::isdigit(currChar)) {
            if (inWord) {
                throw INVALID_WORD_ERROR;
            }
            if (!inNumber) {
                inNumber = true;
            }
            ++data->digits;
            currentNumber += currChar;
        }

        else if (currChar == '.' || currChar == '?' || currChar == '!') {
            ++data->symbols;

            if (currChar == '.' && inNumber) {
                if (i + 1 < currentLine.length() && std::isdigit(currentLine[i + 1])) {
                    currentNumber += currChar;
                    isFloat = true;
                    continue;
                }
                else if (isFloat) {
                    processNumber(currentNumber, data, isFloat);
                }
            }
            if (!isFloat) {
                ++data->sentences;
            }

            processNumber(currentNumber, data, isFloat);
            inWord = false;
            inNumber = false;
        }

        else if (currChar == ' ') {
            ++data->spaces;
            processNumber(currentNumber, data, isFloat);
            inWord = false;
            inNumber = false;
        }

        else {
            ++data->symbols;
            processNumber(currentNumber, data, isFloat);
            inWord = false;
            inNumber = false;
        }
    }

    processNumber(currentNumber, data, isFloat);
}

void Counter::processNumber(std::string& currentNumber, Statistics* data, bool& isFloat) {
    if (!currentNumber.empty()) {
        ++data->numbers;

        if (isFloat) {
            data->floats += parseFloat(currentNumber);
        }
        else {
            data->integers += parseInteger(currentNumber);
        }

        isFloat = false;
        currentNumber.clear();
    }
}

int Counter::parseInteger(const std::string& integerString) {
    try {
        size_t pos;
        int value = std::stoi(integerString, &pos);
        if (pos != integerString.length()) {
            throw INVALID_INT_ERROR;
        }
        return value;
    }

    catch (const std::invalid_argument&) {
        throw INVALID_INT_ERROR;
    }

    catch (const std::out_of_range&) {
        throw INVALID_INT_ERROR;
    }
}

double Counter::parseFloat(const std::string& floatString) {
    try {
        size_t pos;
        double value = std::stof(floatString, &pos);
        if (pos != floatString.length()) {
            throw INVALID_FLOAT_ERROR;
        }
        return value;
    }

    catch (const std::invalid_argument&) {
        throw INVALID_FLOAT_ERROR;
    }

    catch (const std::out_of_range&) {
        throw INVALID_FLOAT_ERROR;
    }
}

void Counter::print(const std::string& filename, const Statistics* data) {
    std::ofstream stream(filename);

    if (!stream.is_open()) {
        throw OUTPUT_ERROR;
    }

    print(stream, data);
}

void Counter::print(std::ostream& stream, const Statistics* data) {
    stream <<  LINES << ": " << data->lines << std::endl;
    stream << SENTENCES << ": " << data->sentences << std::endl;
    stream << WORDS << ": " << data->words << std::endl;
    stream << NUMBERS << ": " << data->numbers << std::endl;
    stream << LETTERS << ": " << data->letters << std::endl;
    stream << DIGITS << ": " << data->digits << std::endl;
    stream << SPACES << ": " << data->spaces << std::endl;
    stream << SYMBOLS << ": " << data->symbols << std::endl;
    stream << INTEGERS << ": " << data->integers << std::endl;
    stream << FLOATS << ": " << std::fixed << std::setprecision(3) << data->floats << std::defaultfloat << std::endl;
}