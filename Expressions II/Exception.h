#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception {
private:
	const char* message_;

public:
	inline Exception(const char* message);

	inline const char* message() const;

	virtual ~Exception() = default;
};

inline Exception::Exception(const char* message) : message_(message) {}

inline const char* Exception::message() const {
	return message_;
}

class EvaluationException : public Exception {
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

#endif // !EXCEPTION_H

