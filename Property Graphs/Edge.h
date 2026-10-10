#ifndef EDGE_H
#define EDGE_H

#include "Node.h"

constexpr char EDGE[] = "edge";
constexpr char OPENING_BRACKET[] = "[";
constexpr char CLOSING_BRACKET[] = "]";
constexpr char DASH[] = "-";
constexpr char ARROW[] = ">";

template <typename EData>
class Edge {
private:
	Identifier id_;
	Identifier source_;
	Identifier target_;
	EData data_;

public:
	Edge(Identifier id, Identifier source, Identifier target, const EData& data);
	Edge(Identifier id, Identifier sourceIdentifier, Identifier targetIdentifier, EData&& data);
	Identifier getId() const;
	Identifier getSource() const;
	Identifier getTarget() const;
	EData& getData();
	const EData& getData() const;
	
	void print(std::ostream& os = std::cout) const;
};

template <typename EData>
inline Edge<EData>::Edge(Identifier id, Identifier source, Identifier target, const EData& data)
	: id_(id), source_(source), target_(target), data_(data) {}

template <typename EData>
inline Edge<EData>::Edge(Identifier id, Identifier source, Identifier target, EData&& data)
	: id_(id), source_(source), target_(target), data_(std::move(data)) {}

template <typename EData>
Identifier Edge<EData>::getId() const {
	return id_;
}

template <typename EData>
Identifier Edge<EData>::getSource() const {
	return source_;
}

template <typename EData>
Identifier Edge<EData>::getTarget() const {
	return target_;
}

template <typename EData>
EData& Edge<EData>::getData() {
	return data_;
}

template <typename EData>
const EData& Edge<EData>::getData() const{
	return data_;
}

template <typename EData>
inline void Edge<EData>::print(std::ostream& os) const {
	os << EDGE << " " << OPENING_PARENTHESIS << source_ << CLOSING_PARANTHESIS << DASH
		<< OPENING_BRACKET << id_ << " " << OPENING_CURLY_BRACES << data_ << CLOSING_CURLY_BRACES << CLOSING_BRACKET
		<< DASH << ARROW << OPENING_PARENTHESIS << target_ << CLOSING_PARANTHESIS;
}

template <typename EData>
std::ostream& operator<<(std::ostream& stream, const Edge<EData>& edge);

template <typename EData>
inline std::ostream& operator<<(std::ostream& stream, const Edge<EData>& edge) {
	edge.print(stream);
	return stream;
}


#endif // !EDGE_H
