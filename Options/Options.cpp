#include "Options.h"

bool process_short_option(const std::string& item, args_t::const_iterator& it, args_t::const_iterator end);
bool process_long_option(const std::string& item, args_t::const_iterator& it, args_t::const_iterator end);
void printValueLine(const std::string& option, const std::string& value);
bool process_value_option(const std::string& option, args_t::const_iterator& it, args_t::const_iterator end, bool isShort);
void process_standalone_value(const std::string& item);
void process_flag_option(const std::string& option);
void process_unknown_option(const std::string& option);

bool process_arguments(const args_t& arguments) {
	bool isSuccess = true;

	for (auto it = arguments.begin(); it != arguments.end(); ++it) {
		const std::string& item = *it;

		if (item[0] == '-') {

			if (item[1] == '-') {
				isSuccess &= process_long_option(item.substr(2), it, arguments.end());
			}

			else {
				isSuccess &= process_short_option(item.substr(1), it, arguments.end());
			}

		}

		else {
			process_standalone_value(item);
		}
	}

	return isSuccess;
}

bool process_short_option(const std::string& item, args_t::const_iterator& it, args_t::const_iterator end) {
	bool isSuccess = true;

	for (size_t i = 0; i < item.length(); ++i) {
		const char option = item[i];

		switch (option) {

		case FLAG_T_SHORT:
		case FLAG_X_SHORT:
		case FLAG_Y_SHORT:
			process_flag_option(std::string(1, option));
			break;

		case OPTION_A_SHORT:
		case OPTION_R_SHORT:
		case OPTION_G_SHORT:
		case OPTION_B_SHORT:
			return isSuccess &= process_value_option(item.substr(i), it, end, true);

		default:
			process_unknown_option(std::string(1, option));
			isSuccess = false;
			break;
		}
	}

	return isSuccess;
}

bool process_long_option(const std::string& item, args_t::const_iterator& it, args_t::const_iterator end) {
	bool isSuccess = true;

	if (item == FLAG_GRAYSCALE_LONG || item == FLAG_TRANSPARENT_LONG) {
		process_flag_option(item);
	}

	else if (item == OPTION_ALPHA_LONG || item == OPTION_BLUE_LONG || item == OPTION_GREEN_LONG || item == OPTION_RED_LONG) {
		isSuccess &= process_value_option(item, it, end, false);
	}

	else {
		process_unknown_option(item);
		isSuccess = false;
	}

	return isSuccess;
}

void printValueLine(const std::string& option, const std::string& value) {
	std::cout << "Value option <" << option << "> detected with value <" << value << ">" << std::endl;
}

bool process_value_option(const std::string& option, args_t::const_iterator& it, args_t::const_iterator end, bool isGrouped) {
	if (isGrouped) {
		if (option.length() > 1) {
			const std::string item = *it;
			printValueLine(option.substr(0, 1), option.substr(1));
			return true;
		}
	}

	if (it + 1 != end) {
		++it;
		const std::string value = *it;
		printValueLine(option, value);
		return true;
	}

	std::cout << "Value option <" << option << "> detected but its value is missing!" << std::endl;
	return false;
}

void process_standalone_value(const std::string& item) {
	std::cout << "Standalone value detected <" << item << ">" << std::endl;
}

void process_flag_option(const std::string& option) {
	std::cout << "Flag option <" << option << "> detected" << std::endl;
}

void process_unknown_option(const std::string& option) {
	std::cout << "Unknown option <" << option << "> found!" << std::endl;
}



