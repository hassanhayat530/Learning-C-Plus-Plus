#ifndef READER_H
#define READER_H

#include <vector>
#include<string>
#include <sstream>
#include <fstream>
#include "Exception.h"

constexpr char ARGUMENT_START = '-';
constexpr char ARGUMENT_MODE = 'a';
constexpr char FILE_MODE = 'f';

constexpr char INVALID_OPTION[] = "Invalid option";
constexpr char FILE_EXCEPTION_MSG[] = "Unable to open input file";

class Reader {
public:
    static void process_arguments(const std::vector<std::string_view>& arguments, std::vector<std::string>& expressions);
	static void process_file(const std::string& filename, std::vector<std::string>& expressions);
};

#endif // !READER_H

