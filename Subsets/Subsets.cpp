#include <iostream>
#include "Input.h"

void subsets(const char items[], size_t count);
void generateSubsets(const char items[], bool signature[], size_t index, size_t count);
void printSubset(const char items[], const bool signature[], size_t count);

int main() {
	subsets(ITEMS, COUNT);
	return 0;
}

void subsets(const char items[], size_t count) {
	bool* signature = new bool[count]();

	generateSubsets(items, signature, 0, count);

	delete[] signature;
}

void generateSubsets(const char items[], bool signature[], size_t index, size_t count) {
    if (index == count) {
        printSubset(items, signature, count);
        return;
    }

    // Include current element
    signature[index] = true;
    generateSubsets(items, signature, index + 1, count);

    // Exclude current element
    signature[index] = false;
    generateSubsets(items, signature, index + 1, count);
}

void printSubset(const char items[], const bool signature[], size_t count) {
    bool atFirstElement = true;

    std::cout << "{";

    for (size_t i = 0; i < count; ++i) {
        if (signature[i] == true) {
            if (!atFirstElement) std::cout << ", ";
            else std::cout << " ";

            std::cout << items[i];

            atFirstElement = false;
        }
    }

    std::cout << " }" << std::endl;
}