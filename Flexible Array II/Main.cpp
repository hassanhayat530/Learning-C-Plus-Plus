#include "Array.h"
#include <iostream>

int main() {
    lib::Array<int> arr = { 1, 2, 3, 4, 5 };
    const lib::Array<int>& const_arr = arr;

    // Non-const iteration
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        *it *= 2;  // Can modify through non-const iterator
    }

    // Const iteration (cannot modify)
    for (auto it = const_arr.begin(); it != const_arr.end(); ++it) {
        // *it = 10;  // Error: cannot modify through const iterator
        std::cout << *it << " ";
    }

    // cbegin/cend for explicit const iteration
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        // *it = 10;  // Error: cannot modify through const iterator
        std::cout << *it << " ";
    }

    // Range-based for loops work with both
    for (int& val : arr) {  // Uses non-const iterators
        val += 1;
    }

    for (const int& val : const_arr) {  // Uses const iterators
        std::cout << val << " ";
    }

    // Conversion from iterator to const_iterator
    lib::Array<int>::iterator non_const_it = arr.begin();
    lib::Array<int>::const_iterator const_it = non_const_it;  // OK: conversion
    // non_const_it = const_it;  // Error: no conversion from const to non-const

    return 0;
}