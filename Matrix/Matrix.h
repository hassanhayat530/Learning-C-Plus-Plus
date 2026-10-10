#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <iostream>
#include <memory>

constexpr char LEFT_BRACKET[] = "[";
constexpr char RIGHT_BRACKET[] = "]";
constexpr char COMMA[] = ",";

// Matrix Class Definition
template<typename Element, size_t Height, size_t Width>
class Matrix {
private:
	std::shared_ptr<std::array<Element, Height * Width>> data_;
	void ensure_ownership();
	class Request;

public:
	Matrix(const Element& value = 0);
	Element& get(size_t row, size_t column);
	const Element& get(size_t row, size_t column) const;
	void set(size_t row, size_t column, const Element& value);
	void print(std::ostream& os = std::cout) const;

	Matrix& operator++();
	Matrix operator++(int);
	Request operator[](size_t row);
	const Request operator[](size_t row) const;
};

// Axillary Class Request Definition
template <typename Element, size_t Height, size_t Width>
class Matrix<Element, Height, Width>::Request {
private:
	const size_t row_;
	const Matrix& matrix_;

	Request(size_t row, Matrix& matrix);
	Request(size_t row, const Matrix& matrix);

	friend class Matrix<Element, Height, Width>;

public:
	Element& operator[](size_t column);
	const Element& operator[](size_t column) const;
};

// Global Function Declarations
template<typename Element, size_t Height, size_t Width>
std::ostream& operator<<( std::ostream& stream, const Matrix<Element, Height, Width>& matrix);

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator+(const Matrix<Element, Height, Width>& matrix, const Element& increment);

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator*(const Matrix<Element, Height, Width>& matrix, const Element& factor);

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator+(const Matrix<Element, Height, Width>& matrix1, const Matrix<Element, Height, Width>& matrix2);

template<typename Element, size_t Height, size_t Depth, size_t Width>
Matrix<Element, Height, Width> operator*(const Matrix<Element, Height, Depth>& matrix1, const Matrix<Element, Depth, Width>& matrix2);

// Matrix Method Definitions
template<typename Element, size_t Height, size_t Width>
inline Matrix<Element, Height, Width>::Matrix(const Element& value)
	: data_(std::make_shared<std::array<Element, Height* Width>>()) {
	data_->fill(value);
}

template<typename Element, size_t Height, size_t Width>
inline void Matrix<Element, Height, Width>::ensure_ownership() {
	if (data_.use_count() > 1) {
		data_ = std::make_shared<std::array<Element, Height* Width>>(*data_);
	}
}

template<typename Element, size_t Height, size_t Width>
Element& Matrix<Element, Height, Width>::get(size_t row, size_t column) {
	ensure_ownership();
	return (*data_)[row * Width + column];
}

template<typename Element, size_t Height, size_t Width>
const Element& Matrix<Element, Height, Width>::get(size_t row, size_t column) const{
	return (*data_)[row * Width + column];
}

template<typename Element, size_t Height, size_t Width>
void Matrix<Element, Height, Width>::set(size_t row, size_t column, const Element& value) {
	ensure_ownership();
	get(row, column) = value;
}

template<typename Element, size_t Height, size_t Width>
void Matrix<Element, Height, Width>::print(std::ostream& os) const {
	os << LEFT_BRACKET;
	for (size_t row = 0; row < Height; ++row) {
		os << LEFT_BRACKET;
		for (size_t column = 0; column < Width; ++column) {
			os << get(row, column);
			if (column + 1 != Width) {
				os << COMMA << " ";
			}
		}
		os << RIGHT_BRACKET;
		if (row + 1 != Height) {
			os << COMMA << " ";
		}
	}
	os << RIGHT_BRACKET;
}

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width>& Matrix<Element, Height, Width>::operator++() {
	ensure_ownership();
	for (auto& elem : *data_) {
		++elem;
	}

	return *this;
}

template<typename Element, size_t Height, size_t Width>
inline Matrix<Element, Height, Width> Matrix<Element, Height, Width>::operator++(int) {
	Matrix temp = *this;
	ensure_ownership();
	for (auto& elem : *data_) {
		elem++;
	}
	return temp;
}

template<typename Element, size_t Height, size_t Width>
inline typename Matrix<Element, Height, Width>::Request Matrix<Element, Height, Width>::operator[](size_t row) {
	return Request(row, *this);
}

template<typename Element, size_t Height, size_t Width>
inline const typename Matrix<Element, Height, Width>::Request Matrix<Element, Height, Width>::operator[](size_t row) const {
	return Request(row, *this);
}

// Request Method Defintions
template<typename Element, size_t Height, size_t Width>
inline Matrix<Element, Height, Width>::Request::Request(size_t row, Matrix& matrix) : row_(row), matrix_(matrix) {}

template<typename Element, size_t Height, size_t Width>
inline Matrix<Element, Height, Width>::Request::Request(size_t row, const Matrix& matrix) : row_(row), matrix_(matrix) {}

template<typename Element, size_t Height, size_t Width>
inline Element& Matrix<Element, Height, Width>::Request::operator[](size_t column) {
	const_cast<Matrix&>(matrix_).ensure_ownership();
	return (*const_cast<Matrix&>(matrix_).data_)[row_ * Width + column];
}

template <typename Element, size_t Height, size_t Width>
inline const Element& Matrix<Element, Height, Width>::Request::operator[](size_t column) const {
	return (*matrix_.data_)[row_ * Width + column];
}

// Global Function Defintions
template<typename Element, size_t Height, size_t Width>
std::ostream& operator<<(std::ostream& stream, const Matrix<Element, Height, Width>& matrix) {
	matrix.print(stream);
	return stream;
}

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator+(const Matrix<Element, Height, Width>& matrix, const Element& increment) {
	Matrix<Element, Height, Width> result = matrix;
	for (size_t row = 0; row < Height; ++row) {
		for (size_t col = 0; col < Width; ++col) {
			result.set(row, col, result[row][col] + increment);
		}
	}
	return result;
}

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator*(const Matrix<Element, Height, Width>& matrix, const Element& factor) {
	Matrix<Element, Height, Width> result = matrix;
	for (size_t row = 0; row < Height; ++row) {
		for (size_t col = 0; col < Width; ++col) {
			result.set(row, col, result[row][col] * factor);
		}
	}
	return result;
}

template<typename Element, size_t Height, size_t Width>
Matrix<Element, Height, Width> operator+(const Matrix<Element, Height, Width>& matrix1, const Matrix<Element, Height, Width>& matrix2) {
	Matrix<Element, Height, Width> result = matrix1;
	for (size_t row = 0; row < Height; ++row) {
		for (size_t col = 0; col < Width; ++col) {
			result.set(row, col, result[row][col] + matrix2[row][col]);
		}
	}
	return result;
}

template<typename Element, size_t Height, size_t Depth, size_t Width>
Matrix<Element, Height, Width> operator*(const Matrix<Element, Height, Depth>& matrix1, const Matrix<Element, Depth, Width>& matrix2) {
	Matrix<Element, Height, Width> result(0);

	for (size_t row = 0; row < Height; ++row) {
		for (size_t col = 0; col < Width; ++col) {
			Element resultElem = 0;
			for (size_t k = 0; k < Depth; ++k) {
				resultElem += matrix1[row][k] * matrix2[k][col];
			}
			result.set(row, col, resultElem);
		}
	}

	return result;
}

#endif // !MATRIX_H
