#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <vector>
#include <new>
#include <stdexcept> 

constexpr size_t BLOCK_SIZE = 10;

template <typename T>
class Array {
private:
	std::vector<T*> data_;
	size_t blockSize_;
	size_t size_;
	size_t capacity_;

	void checkIndex(size_t index) const;
	void addBlock();
	void removeBlock();

	T* getPointer(size_t index);
	const T* getPointer(size_t index) const;

public:
	~Array() noexcept;

	Array(size_t block_size = BLOCK_SIZE);
	Array(const Array& other);
	Array(Array&& other) noexcept;

	size_t size() const;
	bool empty() const;
	size_t capacity() const;

	T& at(size_t index);
	const T& at(size_t index) const;
	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	void push_back(const T& item);
	void push_back(T&& item);

	void pop_back();
	void clear();

	friend void swap(Array& array_1, Array& array_2) noexcept {
		std::swap(array_1.data_, array_2.data_);
		std::swap(array_1.blockSize_, array_2.blockSize_);
		std::swap(array_1.size_, array_2.size_);
		std::swap(array_1.capacity_, array_2.capacity_);
	};

	Array& operator=(const Array& other);
	Array& operator=(Array&& other) noexcept;
};

template <typename T>
Array<T>::Array(size_t blockSize) : blockSize_(blockSize), size_(0), capacity_(0) {}

template <typename T>
Array<T>::Array(const Array& other) : Array(other.blockSize_) {
	try {
		for (size_t i = 0; i < other.size_; ++i) {
			push_back(other.at(i));
		}
	}
	catch (...) {
		clear();
		throw;
	}
}

template <typename T>
inline Array<T>::Array(Array&& other) noexcept
	: data_(std::move(other.data_)), blockSize_(other.blockSize_), size_(other.size_), capacity_(other.capacity_) {
	other.blockSize_ = 0;
	other.size_ = 0;
	other.capacity_ = 0;
}

template <typename T>
inline T* Array<T>::getPointer(size_t index) {
	return &data_[index / blockSize_][index % blockSize_];
}

template <typename T>
inline const T* Array<T>::getPointer(size_t index) const {
	return &data_[index / blockSize_][index % blockSize_];
}

template <typename T>
inline size_t Array<T>::size() const {
	return size_;
}

template <typename T>
inline bool Array<T>::empty() const {
	return size_ == 0;
}

template <typename T>
inline size_t Array<T>::capacity() const {
	return capacity_;
}

template <typename T>
void Array<T>::checkIndex(size_t index) const {
	if (index >= size_) {
		throw std::out_of_range("Invalid index");
	}
}

template <typename T>
inline T& Array<T>::at(size_t index) {
	checkIndex(index);
	return *getPointer(index);
}

template <typename T>
inline const T& Array<T> ::at(size_t index) const {
	checkIndex(index);
	return *getPointer(index);
}

template <typename T>
inline T& Array<T>::operator[](size_t index) {
	#ifdef ARRAY_DEBUG_MODE
		checkIndex(index);
	#endif
	return *getPointer(index);
}

template <typename T>
inline const T& Array<T>::operator[](size_t index) const{
	#ifdef ARRAY_DEBUG_MODE
		checkIndex(index);
	#endif
	return *getPointer(index);
}

template <typename T>
inline void Array<T>::addBlock() {
	T* block = static_cast<T*>(::operator new(blockSize_ * sizeof(T)));

	try {
		data_.push_back(block);
	}
	catch (const std::bad_alloc&) {
		::operator delete(block);
		throw;
	}

	capacity_ += blockSize_;
}

template <typename T>
inline void Array<T>::removeBlock() {
	T* block = data_.back();
	data_.pop_back();
	::operator delete(block);

	capacity_ -= blockSize_;
}

template <typename T>
inline void Array<T>::push_back(const T& item) {
	bool isBlockAdded = false;

	if (size_ == capacity_) {
		addBlock();
		isBlockAdded = true;
	}

	T* target = getPointer(size_);

	try {
		new (target) T(item);
	}
	catch (...) {
		if (isBlockAdded) {
			removeBlock();
		}
		throw;
	}
	++size_;
}

template <typename T>
inline void Array<T>::push_back(T&& item) {
	bool isBlockAdded = false;

	if (size_ == capacity_) {
		addBlock();
		isBlockAdded = true;
	}

	T* target = getPointer(size_);

	try {
		new (target) T(std::move(item));
	}
	catch (...) {
		if (isBlockAdded) {
			removeBlock();
		}
		throw;
	}
	++size_;
}

template <typename T>
inline void Array<T>::pop_back() {
	#ifdef ARRAY_DEBUG_MODE
		if (size_ == 0) {
			throw std::invalid_argument("Empty array");
		}
	#endif

	T* target = getPointer(size_ - 1);
	target->~T();
	--size_;

	if (size_ % blockSize_ == 0 && capacity_ > size_) {
		removeBlock();
	}
}

template <typename T>
inline void Array<T>::clear() {
	for (size_t currentSize = size_; currentSize > 0; --currentSize) {
		pop_back();
	}
}

template<typename T>
inline Array<T>::~Array() noexcept {
	clear();
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
	if (this != &other) {
		Array<T> temp(other);
		swap(*this, temp);
	}
	return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
	if (this != &other) {
		Array<T> temp(std::move(other));
		swap(*this, temp);
	}
	return *this;
}

#endif // !ARRAY_H