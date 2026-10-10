#include <iostream>
#include "Options.h"

int main(int argc, char** argv) {
    args_t arguments(argv + 1, argv + argc);

    bool isSuccess = process_arguments(arguments);

    return isSuccess ? SUCCESS_CODE : FAIL_CODE;
}