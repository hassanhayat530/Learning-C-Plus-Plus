#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <format>

constexpr char NODE[] = "node";
constexpr char OPENING_PARENTHESIS = '(';
constexpr char CLOSING_PARANTHESIS = ')';
constexpr char OPENING_CURLY_BRACES = '{';
constexpr char CLOSING_CURLY_BRACES = '}';

using Identifier = size_t;

template <typename NData>
class Node {
private:
	Identifier id_;
	NData data_;

public:
	Node(Identifier id, const NData& data);
	Node(Identifier id, NData&& data);
	Identifier getId() const;
	NData& getData();
	const NData& getData() const;
	void print(std::ostream& os = std::cout) const;
};

template <typename NData>
inline Node<NData>::Node(Identifier id, const NData& data) : id_(id), data_(data) {}

template <typename NData>
inline Node<NData>::Node(Identifier id, NData&& data) : id_(id), data_(std::move(data)) {}

template <typename NData>
Identifier Node<NData>::getId() const{
	return id_;
}

template <typename NData>
NData& Node<NData>::getData() {
	return data_;
}

template <typename NData>
const NData& Node<NData>::getData() const{
	return data_;
}


template <typename NData>
inline void Node<NData>::print(std::ostream& os) const {
	os << NODE << " " << OPENING_PARENTHESIS << id_ << " " << OPENING_CURLY_BRACES << data_ << CLOSING_CURLY_BRACES << CLOSING_PARANTHESIS;
}

template <typename NData>
std::ostream& operator<<(std::ostream& stream, const Node<NData>& edge);

template <typename NData>
inline std::ostream& operator<<(std::ostream& stream, const Node<NData>& node) {
	node.print(stream);
	return stream;
}


#endif // !NODE_H
