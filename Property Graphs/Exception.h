// Exception.h
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
public:
    virtual const char* message() const = 0;
    virtual ~Exception() = default;
};

class FixedException : public Exception {
private:
    const char* message_;
public:
    FixedException(const char* message);
    const char* message() const override;
};

inline FixedException::FixedException(const char* message) : message_(message) {}

inline const char* FixedException::message() const {
    return message_;
}

class DynamicMessageException : public Exception {
private:
    std::string message_;
public:
    DynamicMessageException(const std::string& message);
    const char* message() const override;
};

inline DynamicMessageException::DynamicMessageException(const std::string& message) : message_(message) {}

inline const char* DynamicMessageException::message() const {
    return message_.c_str();
}

class ElementException final : public DynamicMessageException {
public:
    using DynamicMessageException::DynamicMessageException;
};

class IdentifierException final : public DynamicMessageException {
public:
    using DynamicMessageException::DynamicMessageException;
};

class ConflictException final : public DynamicMessageException {
public:
    using DynamicMessageException::DynamicMessageException;
};

class FileException final : public DynamicMessageException {
public:
    using DynamicMessageException::DynamicMessageException;
};

class MemoryException final : public FixedException {
public:
    using FixedException::FixedException;
};

#endif // EXCEPTION_H

