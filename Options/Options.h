#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>
#include <vector>

// Return Codes
constexpr int SUCCESS_CODE = 0;
constexpr int FAIL_CODE = 1;

// Flags
constexpr char FLAG_TRANSPARENT_LONG[] = "transparent";
constexpr char FLAG_GRAYSCALE_LONG[] = "grayscale";
constexpr char FLAG_T_SHORT = 't';
constexpr char FLAG_X_SHORT = 'x';
constexpr char FLAG_Y_SHORT = 'y';

// Value Options
constexpr char OPTION_RED_LONG[] = "red";
constexpr char OPTION_GREEN_LONG[] = "green";
constexpr char OPTION_BLUE_LONG[] = "blue";
constexpr char OPTION_ALPHA_LONG[] = "alpha";
constexpr char OPTION_R_SHORT = 'r';
constexpr char OPTION_G_SHORT = 'g';
constexpr char OPTION_B_SHORT = 'b';
constexpr char OPTION_A_SHORT = 'a';

using args_t = std::vector<std::string>;

bool process_arguments(const args_t& arguments);

#endif

