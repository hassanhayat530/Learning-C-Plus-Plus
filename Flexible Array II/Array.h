#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <vector>
#include <new>
#include <stdexcept>
#include <cstddef>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <type_traits>

constexpr size_t BLOCK_SIZE = 10;

namespace lib {

    // Concept
    template<typename T>
    concept ArrayElement = (
        std::copy_constructible<T> ||
        std::move_constructible<T> ||
        std::default_initializable<T>
        ) && std::destructible<T>;


    // Flexible Array Class Definition
    template<ArrayElement T>
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
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        template<bool Constant>
        class iterator_base;

        using iterator = iterator_base<false>;
        using const_iterator = iterator_base<true>;

        ~Array() noexcept;

        Array(size_t block_size = BLOCK_SIZE);
        Array(const Array& other);
        Array(Array&& other) noexcept;
        Array(size_t count, const T& item);
        Array(std::initializer_list<T> items);

        template<std::input_iterator InputIterator>
        Array(InputIterator first, InputIterator last);

        size_t size() const;
        bool empty() const;
        size_t capacity() const;

        T& at(size_t index);
        const T& at(size_t index) const;
        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        template<typename Source>
            requires std::constructible_from<T, Source&&>
        void push_back(Source&& item);

        template<typename... Args>
            requires std::constructible_from<T, Args&&...>
        T& emplace_back(Args&&... args);

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

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        void resize(size_t size);

        iterator erase(const_iterator first, const_iterator last);
    };

    // Iterator Class Definition
    template<ArrayElement T>
    template<bool Constant>
    class Array<T>::iterator_base {
    private:
        using array_pointer = std::conditional_t<Constant, const Array<T>*, Array<T>*>;

        array_pointer array_;
        size_t position_;

        friend class Array<T>;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::conditional_t<Constant, const T, T>;
        using difference_type = std::ptrdiff_t;

        using pointer = std::conditional_t<Constant, const T*, T*>;
        using reference = std::conditional_t<Constant, const T&, T&>;

        iterator_base(array_pointer array, size_t position);
        iterator_base();

        operator iterator_base<true>() const requires (!Constant);

        bool operator==(const iterator_base& other) const;
        bool operator!=(const iterator_base& other) const;

        iterator_base& operator++();
        iterator_base operator++(int);
        iterator_base& operator--();
        iterator_base operator--(int);

        reference operator*() const;
        pointer operator->() const;

        iterator_base operator+(difference_type n) const;
        iterator_base operator-(difference_type n) const;
        difference_type operator-(const iterator_base& other) const;
        iterator_base& operator+=(difference_type n);
        iterator_base& operator-=(difference_type n);

        reference operator[](difference_type n) const;

        bool operator<(const iterator_base& other) const;
        bool operator<=(const iterator_base& other) const;
        bool operator>(const iterator_base& other) const;
        bool operator>=(const iterator_base& other) const;

        friend iterator_base operator+(difference_type n, const iterator_base& it) {
            return iterator_base(it.array_, it.position_ + n);
        }
    };

    // ----------------------------------- Iterator Implementation ------------------------------------------------
    template<ArrayElement T>
    template<bool Constant>
    inline Array<T>::iterator_base<Constant>::iterator_base(array_pointer array, size_t position)
        : array_(array), position_(position) {
    }

    template<ArrayElement T>
    template<bool Constant>
    inline Array<T>::iterator_base<Constant>::iterator_base() : array_(nullptr), position_(0) {}

    template<ArrayElement T>
    template<bool Constant>
    inline Array<T>::iterator_base<Constant>::operator iterator_base<true>() const requires (!Constant) {
        return iterator_base<true>(array_, position_);
    }

    template<ArrayElement T>
    template<bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator==(const iterator_base& other) const {
        return array_ == other.array_ && position_ == other.position_;
    }

    template<ArrayElement T>
    template<bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator!=(const iterator_base& other) const {
        return !(*this == other);
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>&
        Array<T>::iterator_base<Constant>::operator++() {
        ++position_;
        return *this;
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>
        Array<T>::iterator_base<Constant>::operator++(int) {
        iterator_base temp = *this;
        ++position_;
        return temp;
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>&
        Array<T>::iterator_base<Constant>::operator--() {
        --position_;
        return *this;
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>
        Array<T>::iterator_base<Constant>::operator--(int) {
        iterator_base temp = *this;
        --position_;
        return temp;
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>::reference
        Array<T>::iterator_base<Constant>::operator*() const {
        return (*array_)[position_];
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>::pointer
        Array<T>::iterator_base<Constant>::operator->() const {
        return &(*array_)[position_];
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>
        Array<T>::iterator_base<Constant>::operator+(difference_type n) const {
        return iterator_base(array_, position_ + n);
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>
        Array<T>::iterator_base<Constant>::operator-(difference_type n) const {
        return iterator_base(array_, position_ - n);
    }

    template<ArrayElement T>
    template<bool Constant>
    inline typename Array<T>::template iterator_base<Constant>::difference_type
        Array<T>::iterator_base<Constant>::operator-(const iterator_base& other) const {
        return static_cast<difference_type>(position_) - static_cast<difference_type>(other.position_);
    }

    template <ArrayElement T>
    template <bool Constant>
    inline typename Array<T>::template iterator_base<Constant>&
        Array<T>::iterator_base<Constant>::operator+=(difference_type n) {
        position_ += n;
        return *this;
    }

    template <ArrayElement T>
    template <bool Constant>
    inline typename Array<T>::template iterator_base<Constant>&
        Array<T>::iterator_base<Constant>::operator-=(difference_type n) {
        position_ -= n;
        return *this;
    }

    template <ArrayElement T>
    template <bool Constant>
    inline typename Array<T>::template iterator_base<Constant>::reference
        Array<T>::iterator_base<Constant>::operator[](difference_type n) const {
        return array_->at(position_ + n);
    }

    template <ArrayElement T>
    template <bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator<(const iterator_base& other) const {
        return position_ < other.position_;
    }

    template <ArrayElement T>
    template <bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator<=(const iterator_base& other) const {
        return position_ <= other.position_;
    }

    template <ArrayElement T>
    template <bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator>(const iterator_base& other) const {
        return position_ > other.position_;
    }

    template <ArrayElement T>
    template <bool Constant>
    inline bool Array<T>::iterator_base<Constant>::operator>=(const iterator_base& other) const {
        return position_ >= other.position_;
    }

    template <ArrayElement T>
    inline typename Array<T>::iterator Array<T>::begin() {
        return iterator(this, 0);
    }

    template <ArrayElement T>
    inline typename Array<T>::iterator Array<T>::end() {
        return iterator(this, size_);
    }

    template <ArrayElement T>
    inline typename Array<T>::const_iterator Array<T>::begin() const {
        return const_iterator(this, 0);
    }

    template <ArrayElement T>
    inline typename Array<T>::const_iterator Array<T>::end() const {
        return const_iterator(this, size_);
    }

    template <ArrayElement T>
    inline typename Array<T>::const_iterator Array<T>::cbegin() const {
        return const_iterator(this, 0);
    }

    template <ArrayElement T>
    inline typename Array<T>::const_iterator Array<T>::cend() const {
        return const_iterator(this, size_);
    }

    // ----------------------------------- Flexible Array Implementation ------------------------------------------------
    template<ArrayElement T>
    Array<T>::Array(size_t blockSize) : blockSize_(blockSize), size_(0), capacity_(0) {}

    template<ArrayElement T>
    inline Array<T>::Array(const Array& other) : Array(other.blockSize_) {
        static_assert(std::copy_constructible<T>,
            "Type T must be copy constructible to use the copy constructor");

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

    template<ArrayElement T>
    inline Array<T>::Array(Array&& other) noexcept
        : data_(std::move(other.data_)), blockSize_(other.blockSize_), size_(other.size_), capacity_(other.capacity_) {
        static_assert(std::move_constructible<T>,
            "Type T must be move constructible to use the move constructor");
        other.blockSize_ = 0;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    template<ArrayElement T>
    inline Array<T>::Array(size_t count, const T& item) : blockSize_(10), size_(0), capacity_(0) {
        static_assert(std::copy_constructible<T>,
            "Array<T>(size_t, const T&): Type T must be copy constructible for repeating constructor");

        try {
            for (size_t i = 0; i < count; ++i) {
                push_back(item);
            }
        }
        catch (...) {
            clear();
            throw;
        }
    }

    template<ArrayElement T>
    inline Array<T>::Array(std::initializer_list<T> items) : blockSize_(10), size_(0), capacity_(0) {
        static_assert(std::copy_constructible<T>,
            "Array<T>(std::initializer_list<T>): Type T must be copy constructible for initializer list constructor");

        try {
            for (auto&& item : items) {
                push_back(item);
            }
        }
        catch (...) {
            clear();
            throw;
        }
    }

    template<ArrayElement T>
    template<std::input_iterator InputIterator>
    inline Array<T>::Array(InputIterator first, InputIterator last)
        : blockSize_(10), size_(0), capacity_(0) {

        static_assert(std::constructible_from<T, typename std::iterator_traits<InputIterator>::reference>,
            "Type T must be constructible from the iterator's reference type");

        try {
            for (InputIterator it = first; it != last; ++it) {
                push_back(*it);
            }
        }
        catch (...) {
            clear();
            throw;
        }
    }

    template <ArrayElement T>
    inline T* Array<T>::getPointer(size_t index) {
        return &data_[index / blockSize_][index % blockSize_];
    }

    template <ArrayElement T>
    inline const T* Array<T>::getPointer(size_t index) const {
        return &data_[index / blockSize_][index % blockSize_];
    }

    template<ArrayElement T>
    inline size_t Array<T>::size() const {
        return size_;
    }

    template<ArrayElement T>
    inline bool Array<T>::empty() const {
        return size_ == 0;
    }

    template<ArrayElement T>
    inline size_t Array<T>::capacity() const {
        return capacity_;
    }

    template<ArrayElement T>
    void Array<T>::checkIndex(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Invalid index");
        }
    }

    template<ArrayElement T>
    inline T& Array<T>::at(size_t index) {
        checkIndex(index);
        return *getPointer(index);
    }

    template<ArrayElement T>
    inline const T& Array<T>::at(size_t index) const {
        checkIndex(index);
        return *getPointer(index);
    }

    template<ArrayElement T>
    inline T& Array<T>::operator[](size_t index) {
#ifdef ARRAY_DEBUG_MODE
        checkIndex(index);
#endif
        return at(index);
    }

    template<ArrayElement T>
    inline const T& Array<T>::operator[](size_t index) const {
#ifdef ARRAY_DEBUG_MODE
        checkIndex(index);
#endif
        return at(index);
    }

    template<ArrayElement T>
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

    template<ArrayElement T>
    inline void Array<T>::removeBlock() {
        T* block = data_.back();
        data_.pop_back();
        ::operator delete(block);

        capacity_ -= blockSize_;
    }

    template<ArrayElement T>
    template<typename Source>
        requires std::constructible_from<T, Source&&>
    inline void Array<T>::push_back(Source&& item) {
        emplace_back(std::forward<Source>(item));
    }


    template<ArrayElement T>
    template<typename... Args>
        requires std::constructible_from<T, Args&&...>
    inline T& Array<T>::emplace_back(Args&&... args) {
        bool isBlockAdded = false;

        if (size_ == capacity_) {
            addBlock();
            isBlockAdded = true;
        }

        T* target = getPointer(size_);

        try {
            new (target) T(std::forward<Args>(args)...);
        }
        catch (...) {
            if (isBlockAdded) {
                removeBlock();
            }
            throw;
        }
        ++size_;
        return *target;
    }

    template<ArrayElement T>
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

    template<ArrayElement T>
    inline void Array<T>::clear() {
        for (size_t currentSize = size_; currentSize > 0; --currentSize) {
            pop_back();
        }
    }

    template<ArrayElement T>
    inline Array<T>::~Array() noexcept {
        clear();
    }

    template<ArrayElement T>
    Array<T>& Array<T>::operator=(const Array& other) {
        static_assert(std::copy_constructible<T>,
            "Type T must be copy constructible for copy assignment");

        if (this != &other) {
            Array<T> temp(other);
            swap(*this, temp);
        }
        return *this;
    }

    template<ArrayElement T>
    Array<T>& Array<T>::operator=(Array&& other) noexcept {
        static_assert(std::move_constructible<T>,
            "Type T must be move constructible for move assignment");
        if (this != &other) {
            Array<T> temp(std::move(other));
            swap(*this, temp);
        }
        return *this;
    }

    template<ArrayElement T>
    inline void Array<T>::resize(size_t new_size) {
        static_assert(std::default_initializable<T>,
            "Type T must be default constructible to use resize");

        if (new_size == size_) {
            return;
        }

        if (new_size < size_) {
            while (size_ > new_size) {
                pop_back();
            }
        }
        else {
            size_type original_size = size_;

            while (capacity_ < new_size) {
                addBlock();
            }

            try {
                while (size_ < new_size) {
                    emplace_back();
                }
            }
            catch (...) {
                while (size_ > original_size) {
                    pop_back();
                }
                throw;
            }
        }
    }

    template<ArrayElement T>
    inline typename Array<T>::iterator Array<T>::erase(const_iterator first, const_iterator last) {
        static_assert(std::assignable_from<T&, T&&>,
            "Type T must be move assignable to use erase");

        static_assert(requires(T & t, T && s) { { t = std::move(s) } noexcept; },
            "Move assignment must be noexcept for erase");

        size_t first_pos = first.position_;
        size_t last_pos = last.position_;

        if (first_pos >= last_pos || last_pos > size_) {
            return iterator(this, first_pos);
        }

        size_t elements_to_remove = last_pos - first_pos;

        for (size_t i = last_pos; i < size_; ++i) {
            size_t target_idx = first_pos + (i - last_pos);
            (*this)[target_idx] = std::move((*this)[i]);
        }

        for (size_t i = size_ - elements_to_remove; i < size_; ++i) {
            T* target = &data_[i / blockSize_][i % blockSize_];
            target->~T();
        }

        size_ -= elements_to_remove;

        while (size_ % blockSize_ == 0 && capacity_ > size_) {
            removeBlock();
        }

        return iterator(this, first_pos);
    }

} // namespace lib

#endif // !ARRAY_H