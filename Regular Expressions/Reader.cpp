#include "Reader.h"

void Reader::process_arguments(const std::vector<std::string_view>& arguments, std::vector<std::string>& expressions) {
	bool isArgumentMode = true;

	for (size_t i = 0; i < arguments.size(); ++i) {
		const std::string_view& arg = arguments[i];

		if (arg[0] == ARGUMENT_START) {
			if (arg[1] == ARGUMENT_MODE) {
				isArgumentMode = true;
			}

			else if (arg[1] == FILE_MODE) {
				isArgumentMode = false;
			}

			else {
				throw ArgumentException(INVALID_OPTION);
			}
		}

		else if (isArgumentMode) {
			expressions.push_back(std::string(arg));
		}

		else {
			process_file(std::string(arg), expressions);
		}
	}
}

void Reader::process_file(const std::string& filename, std::vector<std::string>& expressions) {
	std::fstream stream;
	stream.open(filename);

	if (!stream.good()) {
		throw FileException(FILE_EXCEPTION_MSG);
	}

	std::string line;
	while (std::getline(stream, line)) {
		if (!line.empty()) {
			expressions.push_back(line);
		}
	}

	stream.close();
}