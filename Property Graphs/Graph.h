#ifndef GRAPH_H
#define GRAPH_H

#include "Array.h"
#include "Edge.h"
#include "Exception.h"
#include <fstream>
#include <sstream>

constexpr char NODES_MEMORY_EXCEPTION[] = "Unavailable memory for a new node insertion";
constexpr char MATRIX_MEMORY_EXCEPTION[] = "Unavailable memory for the adjacency matrix extension";
constexpr char EDGES_MEMORY_EXCEPTION[] = "Unavailable memory for a new edge insertion";
constexpr char COPY_CONSTRUCTION_EXCEPTION[] = "Unavailable memory for creating a graph copy";
constexpr char COPY_ASSIGNMENT_EXCEPTION[] = "Unavailable memory for assigning a graph copy";

constexpr int NO_EDGE = -1;

enum class Type {
	Directed,
	Undirected,
};

// Graph ---------------------------------------------------------------------------------
template <typename NData, typename EData>
class Graph {
private:
	class Nodes;
	class Edges;

	Nodes nodes_;
	Edges edges_;
	lib::Array<lib::Array<int>> adjacencyMatrix_;
public:
	virtual Type get_type() const = 0;
	virtual ~Graph() = default;

	Graph();
	Graph(const Graph& other);
	Graph(Graph&& other) noexcept;
	Graph& operator=(const Graph& other);
	Graph& operator=(Graph&& other) noexcept;

	Nodes& nodes();
	const Nodes& nodes() const;
	Edges& edges();
	const Edges& edges() const;

	void print(std::ostream& stream = std::cout) const;
	void print(const std::string& filename) const;

	void import(std::istream& stream = std::cin);
	void import(std::string_view filename);
	void parseLine(std::string& line);

	void clear();
};
// ---------------------------------------------------------------------------------------

// ---Constructors---
template<typename NData, typename EData>
Graph<NData, EData>::Graph() :
	nodes_(lib::Array<Node<NData>>(), *this),
	edges_(lib::Array<Edge<EData>>(), *this),
	adjacencyMatrix_() {
}

template <typename NData, typename EData>
Graph<NData, EData>::Graph(const Graph& other)
	: nodes_(lib::Array<Node<NData>>(), *this),
	edges_(lib::Array<Edge<EData>>(), *this),
	adjacencyMatrix_() {
	try {
		adjacencyMatrix_ = other.adjacencyMatrix_;
		nodes_.nodes_ = other.nodes_.nodes_;
		edges_.edges_ = other.edges_.edges_;
	}
	catch (const std::bad_alloc&) {
		clear();
		throw MemoryException(COPY_CONSTRUCTION_EXCEPTION);
	}
	catch (...) {
		throw;
	}
}

template <typename NData, typename EData>
Graph<NData, EData>::Graph(Graph&& other) noexcept
	: nodes_(lib::Array<Node<NData>>(), *this),
	edges_(lib::Array<Edge<EData>>(), *this),
	adjacencyMatrix_() {
	std::swap(adjacencyMatrix_, other.adjacencyMatrix_);
	std::swap(nodes_.nodes_, other.nodes_.nodes_);
	std::swap(edges_.edges_, other.edges_.edges_);
	other.clear();
}

template <typename NData, typename EData>
Graph<NData, EData>& Graph<NData, EData>::operator=(const Graph& other) {
	if (this != &other) {
		lib::Array<lib::Array<int>> tempAdjacencyMatrix;
		lib::Array<Node<NData>> tempNodes;
		lib::Array<Edge<EData>> tempEdges;

		try {
			tempAdjacencyMatrix = other.adjacencyMatrix_;
			tempNodes = other.nodes_.nodes_;
			tempEdges = other.edges_.edges_;
		}
		catch (const std::bad_alloc&) {
			throw MemoryException(COPY_ASSIGNMENT_EXCEPTION);
		}

		adjacencyMatrix_ = std::move(tempAdjacencyMatrix);
		nodes_.nodes_ = std::move(tempNodes);
		nodes_.graph_ = *this;
		edges_.edges_ = std::move(tempEdges);
		edges_.graph_ = *this;
	}
	return *this;
}

template <typename NData, typename EData>
Graph<NData, EData>& Graph<NData, EData>::operator=(Graph&& other) noexcept {
	if (this != &other) {
		std::swap(adjacencyMatrix_, other.adjacencyMatrix_);
		std::swap(nodes_.nodes_, other.nodes_.nodes_);
		nodes_.graph_ = *this;
		std::swap(edges_.edges_, other.edges_.edges_);
		edges_.graph_ = *this;

		other.clear();
	}
	return *this;
}

// ---Accessors---
template <typename NData, typename EData>
inline typename Graph<NData, EData>::Nodes&
Graph<NData, EData>::nodes() {
	return nodes_;
}

template <typename NData, typename EData>
inline const typename Graph<NData, EData>::Nodes&
Graph<NData, EData>::nodes() const {
	return nodes_;
}

template <typename NData, typename EData>
inline typename Graph<NData, EData>::Edges&
Graph<NData, EData>::edges() {
	return edges_;
}

template <typename NData, typename EData>
inline const typename Graph<NData, EData>::Edges&
Graph<NData, EData>::edges() const {
	return edges_;
}

// ---print---
template <typename NData, typename EData>
inline void Graph<NData, EData>::print(std::ostream& stream) const {
	nodes_.print(stream);
	edges_.print(stream);
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::print(const std::string& filename) const {
	std::fstream fileStream;
	fileStream.open(filename, std::ios::out);

	if (!fileStream.good()) {
		throw FileException("Unable to open output file " + static_cast<std::string>(filename));
	}

	print(fileStream);

	fileStream.close();
}

template <typename NData, typename EData>
std::ostream& operator<<(std::ostream& stream, const Graph<NData, EData>& graph);

template <typename NData, typename EData>
inline std::ostream& operator<<(std::ostream& stream, const Graph<NData, EData>& graph) {
	graph.print(stream);
	return stream;
}

// ---import---
template <typename NData, typename EData>
inline void Graph<NData, EData>::import(std::istream& stream) {
	try {
		std::string line;
		while (std::getline(stream, line)) {
			if (!line.empty()) {
				parseLine(line);
			}
		}
	}
	catch (const std::bad_alloc&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
	catch (const MemoryException&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::import(std::string_view filename) {
	try {
		std::fstream fileStream;
		fileStream.open(filename.data());

		if (!fileStream.good()) {
			throw FileException("Unable to open input file " + static_cast<std::string>(filename));
		}

		import(fileStream);
		fileStream.close();
	}
	catch (const FileException&) {
		throw;
	}
	catch (const std::bad_alloc&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
	catch (const MemoryException&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::parseLine(std::string& line) {
	try {
		std::istringstream lineStream(line);
		std::string type;
		lineStream >> type;
		if (type == NODE) {
			char openParenthesis, openBraces;
			Identifier id;
			std::string dataContent;

			lineStream >> openParenthesis >> id >> openBraces;

			std::getline(lineStream, dataContent, CLOSING_CURLY_BRACES);
			std::istringstream dataStream(dataContent);
			NData data;
			dataStream >> data;

			nodes_.add(id, std::move(data));
		}
		else if (type == EDGE) {
			char openParenthesis, closeParenthesis, dash1, openBracket, openBraces, closeBracket, dash2, arrow;
			Identifier id, source, target;
			std::string dataContent;

			lineStream >> openParenthesis >> source >> closeParenthesis >> dash1 >> openBracket >> id >> openBraces;

			std::getline(lineStream, dataContent, CLOSING_CURLY_BRACES);

			std::istringstream dataStream(dataContent);
			EData data;
			dataStream >> data;

			lineStream >> closeBracket >> dash2 >> arrow >> openParenthesis >> target;

			edges_.add(id, source, target, std::move(data));
		}
	}
	catch (const std::bad_alloc&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
	catch (const MemoryException&) {
		throw MemoryException("Unavailable memory for importing graph content");
	}
}

// ---clear---

template <typename NData, typename EData>
inline void Graph<NData, EData>::clear() {
	adjacencyMatrix_.clear();
	nodes_.nodes_.clear();
	edges_.edges_.clear();
}

// Nodes ---------------------------------------------------------------------------------
template <typename NData, typename EData>
class Graph<NData, EData>::Nodes {
private:
	lib::Array<Node<NData>> nodes_;
	Graph& graph_;
	void resizeMatrix();
	friend class Graph;

	void handleInvalidException(Identifier id) const;
	void handleConflictException(Identifier id) const;
	void handleNonSuccessiveException(Identifier id) const;
	void handleAddExceptions(Identifier id) const;
	void handleGetException(Identifier id) const;

	void rollbackAdd(size_t orignalSize);
public:
	Nodes(lib::Array<Node<NData>> nodes, Graph<NData, EData>& graph);

	void print(std::ostream& stream = std::cout) const;

	friend inline std::ostream& operator<<(std::ostream& stream, const typename Graph<NData, EData>::Nodes& nodes) {
		nodes.print(stream);
		return stream;
	}

	Node<NData>& add(Identifier id, const NData& data);
	Node<NData>& add(Identifier id, NData&& data);
	Node<NData>& add(const NData& data);
	Node<NData>& add(NData&& data);

	size_t size() const;
	bool exists(Identifier id) const;

	Node<NData>& get(Identifier id);
	const Node<NData>& get(Identifier id) const;

	Node<NData>& operator[](size_t id);
	const Node<NData>& operator[](size_t id) const;

	auto begin();
	auto end();
	auto begin() const;
	auto end() const;

	void clear();
};
// ---------------------------------------------------------------------------------------

// ---Constructor---
template <typename NData, typename EData>
inline Graph<NData, EData>::Nodes::Nodes(lib::Array<Node<NData>> nodes, Graph<NData, EData>& graph) : nodes_(nodes), graph_(graph) {}

// ---print---
template <typename NData, typename EData>
inline void Graph<NData, EData>::Nodes::print(std::ostream& stream) const {
	for (const auto& node : nodes_) {
		stream << node << std::endl;
	}
}

// ---Exception Handling---
template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::handleInvalidException(Identifier id) const {
	if (id >= SIZE_MAX) {
		throw IdentifierException("Invalid node identifier " + std::to_string(id) + " requested");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::handleConflictException(Identifier id) const {
	if (id < nodes_.size()) {
		throw ConflictException("Node with identifier " + std::to_string(id) + " already exists");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::handleNonSuccessiveException(Identifier id) const {
	if (id - nodes_.size() > 0) {
		throw IdentifierException("Non-successive node identifier " + std::to_string(id) + " requested");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::handleAddExceptions(Identifier id) const {
	handleInvalidException(id);
	handleConflictException(id);
	handleNonSuccessiveException(id);
}


template <typename NData, typename EData>
void Graph<NData, EData>::
Nodes::handleGetException(Identifier id) const {
	if (!exists(id)) {
		throw ElementException("Node with identifier " + std::to_string(id) + " does not exist");
	}
}

// ---Node Addition---
template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::resizeMatrix() {
	try {
		lib::Array<int> newRow(nodes_.size() - 1, NO_EDGE);
		size_t rowsModified = 0;

		try {
			graph_.adjacencyMatrix_.push_back(std::move(newRow));

			for (auto& row : graph_.adjacencyMatrix_) {
				row.push_back(NO_EDGE);
				rowsModified++;
			}
		}
		catch (const std::bad_alloc&) {
			for (size_t i = 0; i < rowsModified; ++i) {
				graph_.adjacencyMatrix_[i].pop_back();
			}
			if (!graph_.adjacencyMatrix_.empty() &&
				graph_.adjacencyMatrix_[graph_.adjacencyMatrix_.size() - 1].size() == nodes_.size() - 1) {
				graph_.adjacencyMatrix_.pop_back();
			}
			throw;
		}
	}
	catch (const std::bad_alloc&) {
		throw MemoryException(NODES_MEMORY_EXCEPTION);
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::rollbackAdd(size_t originalSize) {
	while (nodes_.size() > originalSize) {
		nodes_.pop_back();
	}
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::
Nodes::add(Identifier id, const NData& data) {
	handleAddExceptions(id);

	size_t originalSize = nodes_.size();

	try {
		nodes_.emplace_back(id, data);
		resizeMatrix();
	}
	catch (const MemoryException&) {
		rollbackAdd(originalSize);
		throw;
	}
	catch (const std::bad_alloc&) {
		rollbackAdd(originalSize);
		throw MemoryException(NODES_MEMORY_EXCEPTION);
	}
	catch (...) {
		rollbackAdd(originalSize);
		throw;
	}

	return nodes_[nodes_.size() - 1];
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::
Nodes::add(Identifier id, NData&& data) {
	handleAddExceptions(id);

	size_t originalSize = nodes_.size();

	try {
		nodes_.emplace_back(id, std::move(data));
		resizeMatrix();
	}
	catch (const MemoryException&) {
		rollbackAdd(originalSize);
		throw;
	}
	catch (const std::bad_alloc&) {
		rollbackAdd(originalSize);
		throw MemoryException(NODES_MEMORY_EXCEPTION);
	}
	catch (...) {
		rollbackAdd(originalSize);
		throw;
	}

	return nodes_[nodes_.size() - 1];
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::
Nodes::add(const NData& data) {
	return add(nodes_.size(), data);
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::
Nodes::add(NData&& data) {
	return add(nodes_.size(), std::move(data));
}

// ---Accessors---
template <typename NData, typename EData>
inline size_t Graph<NData, EData>::Nodes::size() const {
	return nodes_.size();
}

template <typename NData, typename EData>
inline bool Graph<NData, EData>::Nodes::exists(Identifier id) const {
	if (size() <= id) {
		return false;
	}

	return true;
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::Nodes::get(Identifier id) {
	handleGetException(id);
	return nodes_[id];
}

template <typename NData, typename EData>
inline const Node<NData>& Graph<NData, EData>::Nodes::get(Identifier id) const {
	handleGetException(id);
	return nodes_[id];
}

template <typename NData, typename EData>
inline Node<NData>& Graph<NData, EData>::Nodes::operator[](size_t id) {
	return get(id);
}

template <typename NData, typename EData>
inline const Node<NData>& Graph<NData, EData>::Nodes::operator[](size_t id) const {
	return get(id);
}

// ---Iterator Implementation---
template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Nodes::begin() {
	return nodes_.begin();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Nodes::begin() const {
	return nodes_.begin();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Nodes::end() {
	return nodes_.end();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Nodes::end() const {
	return nodes_.end();
}

// ---clear---
template <typename NData, typename EData>
inline void Graph<NData, EData>::
Nodes::clear() {
	nodes_.clear();
}

// Edges ---------------------------------------------------------------------------------
template <typename NData, typename EData>
class Graph<NData, EData>::Edges {
private:
	lib::Array<Edge<EData>> edges_;
	Graph& graph_;

	class Request;
	friend class Graph;

	void handleInvalidException(Identifier id) const;
	void handleConflictException(Identifier id) const;
	void handleNonSuccessiveException(Identifier id) const;
	void handleAlreadyExistsException(Identifier source, Identifier target) const;
	void handleGetException(Identifier id) const;
	void handleSourceException(Identifier source) const;
	void handleTargetException(Identifier target) const;
	void handleMissingEdgeException(Identifier source, Identifier target) const;
	void handlePairedGetException(Identifier source, Identifier target) const;
	void handleAddExceptions(Identifier id, Identifier source, Identifier target) const;

	void addMatrixEdge(Identifier id, Identifier source, Identifier target);
	void handleAddMemoryException(Identifier id, Identifier source, Identifier target, size_t originalSize);
public:
	Edges(lib::Array<Edge<EData>> edges, Graph& graph);

	void print(std::ostream& stream = std::cout) const;
	void printMatrix(std::ostream& stream = std::cout) const;

	friend std::ostream& operator<<(std::ostream& stream, const typename Graph<NData, EData>::Edges& edges) {
		edges.print(stream);
		return stream;
	}

	Edge<EData>& add(Identifier id, Identifier source, Identifier target, const EData& data);
	Edge<EData>& add(Identifier id, Identifier source, Identifier target, EData&& data);
	Edge<EData>& add(Identifier source, Identifier target, const EData& data);
	Edge<EData>& add(Identifier source, Identifier target, EData&& data);

	size_t size() const;
	bool exists(Identifier id) const;
	bool exists(Identifier source, Identifier target) const;

	Edge<EData>& get(Identifier id);
	const Edge<EData>& get(Identifier id) const;
	Edge<EData>& get(Identifier source, Identifier target);
	const Edge<EData>& get(Identifier source, Identifier target) const;

	Request operator[](size_t row);
	const Request operator[](size_t row) const;

	auto begin();
	auto end();
	auto begin() const;
	auto end() const;

	void clear();
};

template <typename NData, typename EData>
class Graph<NData, EData>::Edges::Request {
private:
	const size_t row_;
	const Edges& edges_;

	Request(size_t row, Edges& edges);
	Request(size_t row, const Edges& edges);

	friend class Edges;

public:
	Edge<EData>& operator[](size_t col);
	const Edge<EData>& operator[](size_t col) const;
};
// ---------------------------------------------------------------------------------------

// ---Request Class Methods---
template <typename NData, typename EData>
inline Graph<NData, EData>::Edges::Request::Request(size_t row, Edges& edges) : row_(row), edges_(edges) {}

template <typename NData, typename EData>
inline Graph<NData, EData>::Edges::Request::Request(size_t row, const Edges& edges) : row_(row), edges_(edges) {}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::Request::operator[](size_t col) {
	return const_cast<Edges&>(edges_).get(row_, col);
}

template <typename NData, typename EData>
inline const Edge<EData>& Graph<NData, EData>::Edges::Request::operator[](size_t col) const {
	return edges_.get(row_, col);
}

// ---Constructor---
template <typename NData, typename EData>
inline Graph<NData, EData>::Edges::Edges(lib::Array<Edge<EData>> edges, Graph& graph) : edges_(edges), graph_(graph) {}

// ---print---
template <typename NData, typename EData>
inline void Graph<NData, EData>::Edges::print(std::ostream& stream) const {
	for (const auto& edge : edges_) {
		stream << edge << std::endl;
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::printMatrix(std::ostream& stream) const {
	for (const auto& row : graph_.adjacencyMatrix_) {
		for (size_t col = 0; col < row.size(); ++col) {
			if (col > 0) {
				stream << "|";
			}
			if (row[col] == NO_EDGE) {
				stream << "-";
			}
			else {
				stream << std::to_string(row[col]);
			}
		}
		stream << "\n";
	}
}

// ---Exception Handling---
template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleInvalidException(Identifier id) const {
	if (id >= SIZE_MAX) {
		throw IdentifierException("Invalid edge identifier " + std::to_string(id) + " requested");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleConflictException(Identifier id) const {
	if (id < edges_.size()) {
		throw ConflictException("Edge with identifier " + std::to_string(id) + " already exists");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleNonSuccessiveException(Identifier id) const {
	if (id - edges_.size() > 0) {
		throw IdentifierException("Non-successive edge identifier " + std::to_string(id) + " requested");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleAlreadyExistsException(Identifier source, Identifier target) const {
	if (exists(source, target)) {
		throw ConflictException("Edge between nodes " + std::to_string(source) + " and " + std::to_string(target) + " already exists");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleGetException(Identifier id) const {
	if (!exists(id)) {
		throw ElementException("Edge with identifier " + std::to_string(id) + " does not exist");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleSourceException(Identifier source) const {
	if (graph_.adjacencyMatrix_.size() - 1 < source) {
		throw ElementException("Source node with identifier " + std::to_string(source) + " does not exist");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleTargetException(Identifier target) const {
	if (graph_.adjacencyMatrix_.size() - 1 < target) {
		throw ElementException("Target node with identifier " + std::to_string(target) + " does not exist");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleMissingEdgeException(Identifier source, Identifier target) const {
	if (graph_.adjacencyMatrix_[source][target] == NO_EDGE) {
		throw ElementException("Edge between nodes " + std::to_string(source) + " and " + std::to_string(target) + " does not exist");
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handlePairedGetException(Identifier source, Identifier target) const {
	handleSourceException(source);
	handleTargetException(target);
	handleMissingEdgeException(source, target);
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::handleAddExceptions(Identifier id, Identifier source, Identifier target) const {
	handleInvalidException(id);
	handleConflictException(id);
	handleNonSuccessiveException(id);
	handleSourceException(source);
	handleTargetException(target);
	handleAlreadyExistsException(source, target);
}

// ---Edge Addition---

template <typename NData, typename EData>
inline void Graph<NData, EData>::Edges::addMatrixEdge(Identifier id, Identifier source, Identifier target) {
	graph_.adjacencyMatrix_[source][target] = static_cast<int>(id);
	if (graph_.get_type() == Type::Undirected) {
		graph_.adjacencyMatrix_[target][source] = static_cast<int>(id);
	}
}

template <typename NData, typename EData>
inline void Graph<NData, EData>::Edges::handleAddMemoryException(Identifier id, Identifier source, Identifier target, size_t originalSize) {
	while (edges_.size() > originalSize) {
		edges_.pop_back();
	}
	if (originalSize < edges_.size() || graph_.adjacencyMatrix_[source][target] == static_cast<int>(id)) {
		graph_.adjacencyMatrix_[source][target] = NO_EDGE;
		if (graph_.get_type() == Type::Undirected) {
			graph_.adjacencyMatrix_[target][source] = NO_EDGE;
		}
	}
	throw MemoryException(EDGES_MEMORY_EXCEPTION);
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::add(Identifier id, Identifier source, Identifier target, const EData& data) {
	handleAddExceptions(id, source, target);

	size_t originalSize = edges_.size();

	try {
		edges_.emplace_back(id, source, target, data);
		addMatrixEdge(id, source, target);
	}
	catch (const std::bad_alloc&) {
		handleAddMemoryException(id, source, target, originalSize);
	}
	catch (...) {
		while (edges_.size() > originalSize) {
			edges_.pop_back();
		}
		throw;
	}

	return edges_[edges_.size() - 1];
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::add(Identifier id, Identifier source, Identifier target, EData&& data) {
	handleAddExceptions(id, source, target);

	size_t originalSize = edges_.size();

	try {
		edges_.emplace_back(id, source, target, std::move(data));
		addMatrixEdge(id, source, target);
	}
	catch (const std::bad_alloc&) {
		handleAddMemoryException(id, source, target, originalSize);
	}
	catch (...) {
		while (edges_.size() > originalSize) {
			edges_.pop_back();
		}
		throw;
	}

	return edges_[edges_.size() - 1];
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::add(Identifier source, Identifier target, const EData& data) {
	return add(edges_.size(), source, target, data);
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::add(Identifier source, Identifier target, EData&& data) {
	return add(edges_.size(), source, target, std::move(data));
}

// ---Accessors---
template <typename NData, typename EData>
inline size_t Graph<NData, EData>::Edges::size() const {
	return edges_.size();
}

template <typename NData, typename EData>
inline bool Graph<NData, EData>::Edges::exists(Identifier id) const {
	if (size() <= id) {
		return false;
	}
	return true;
}

template <typename NData, typename EData>
inline bool Graph<NData, EData>::Edges::exists(Identifier source, Identifier target) const {
	handleSourceException(source);
	handleTargetException(target);
	if (graph_.adjacencyMatrix_[source][target] == NO_EDGE) {
		return false;
	}
	return true;
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::get(Identifier id) {
	handleGetException(id);
	return edges_[id];
}

template <typename NData, typename EData>
inline const Edge<EData>& Graph<NData, EData>::Edges::get(Identifier id) const {
	handleGetException(id);
	return edges_[id];
}

template <typename NData, typename EData>
inline Edge<EData>& Graph<NData, EData>::Edges::get(Identifier source, Identifier target) {
	handlePairedGetException(source, target);
	return get(graph_.adjacencyMatrix_[source][target]);
}

template <typename NData, typename EData>
inline const Edge<EData>& Graph<NData, EData>::Edges::get(Identifier source, Identifier target) const {
	handlePairedGetException(source, target);
	return get(graph_.adjacencyMatrix_[source][target]);
}

template <typename NData, typename EData>
inline typename Graph<NData, EData>::Edges::Request
Graph<NData, EData>::Edges::operator[](size_t row) {
	handleSourceException(row);
	return Request(row, *this);
}

template <typename NData, typename EData>
inline const typename Graph<NData, EData>::Edges::Request
Graph<NData, EData>::Edges::operator[](size_t row) const {
	return Request(row, *this);
}

//---Iterator Implementation---
template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Edges::begin() {
	return edges_.begin();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Edges::begin() const {
	return edges_.begin();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Edges::end() {
	return edges_.end();
}

template <typename NData, typename EData>
inline auto Graph<NData, EData>::
Edges::end() const {
	return edges_.end();
}

// ---clear---
template <typename NData, typename EData>
inline void Graph<NData, EData>::
Edges::clear() {
	edges_.clear();
}


// Directed Graph --------------------------------------------------------------
template<typename NData, typename EData>
class DirectedGraph : public Graph<NData, EData> {
public:
	using Graph<NData, EData>::Graph;

	Type get_type() const override;
};
// -----------------------------------------------------------------------------

template<typename NData, typename EData>
inline Type DirectedGraph<NData, EData>::get_type() const {
	return Type::Directed;
}

// Undirected Graph ------------------------------------------------------------
template<typename NData, typename EData>
class UndirectedGraph : public Graph<NData, EData> {
	using Graph<NData, EData>::Graph;

	Type get_type() const override;
};

template<typename NData, typename EData>
inline Type UndirectedGraph<NData, EData>::get_type() const {
	return Type::Undirected;
}

// -----------------------------------------------------------------------------

#endif // !GRAPH_H