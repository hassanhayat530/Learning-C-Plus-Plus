#include "Counter.h"

int main() {
	Statistics* data = new Statistics();
	const std::string inputFile = "Input.txt";

	Counter::process(inputFile, data);
	Counter::print(std::cout, data);

	delete data;
}