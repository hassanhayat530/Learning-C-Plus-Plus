#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception {
private:
	const char* message_;

public:
	virtual ~Exception() = default;
	inline Exception(const char* message);
	const char* message() const;
};

class ArgumentException : public Exception {
public:
	using Exception::Exception;
};

class FileException : public Exception {
public:
	using Exception::Exception;
};

class ParseException : public Exception {
public:
	using Exception::Exception;
};

class MemoryException : public Exception {
public:
	using Exception::Exception;
};

inline Exception::Exception(const char* message) : message_(message) {}

inline const char* Exception::message() const {
	return message_;
}

#endif


