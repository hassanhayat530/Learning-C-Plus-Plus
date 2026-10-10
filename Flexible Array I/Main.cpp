#include "Array.h"

int main() {
	Array<int> arr1;
	Array<int> arr2;
	arr1.push_back(1);
	arr2.push_back(2);
	std::swap(arr1, arr2);
	std::cout << arr1[0] << arr2[0];
}