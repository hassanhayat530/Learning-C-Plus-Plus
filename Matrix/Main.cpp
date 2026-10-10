#include "Matrix.h"

int main() {
	Matrix<int, 3, 3> matrix(0);

	matrix.get(0, 0) = 1;
	std::cout << matrix.get(0, 0) << std::endl;

	matrix.print();
	std::cout << std::endl;

	Matrix<int, 3, 3> matrix_incr = matrix + 3;
	matrix_incr.print();

	matrix.print();
}