# Learning C++

A collection of intermediate C++ assignments from a university programming course. Each project lives in its own folder in the root of the repository.

## Projects

### Basics

| Folder | Description |
|---|---|
| `Subsets` | Prints all subsets of a character array using a depth-first search over a dynamically allocated bool mask. The output format is `{ A, C, D }`, and the code is split into a public function and an internal recursive one. |
| `Options` | Parses command-line arguments, recognizing short and long flag options, value options, grouped short options, and standalone values. It reports each item as it is found and exits with code 1 if any option is unknown or a value is missing. |
| `Counter` | Provides a `Counter` class and a `Statistics` struct that compute text statistics (lines, sentences, words, numbers, letters, digits, spaces, symbols, and sums of integers and decimals) from any input stream or file. Malformed input and file errors throw text exceptions. |

### Movie Database

The three movie database projects build on each other.

| Folder | Description |
|---|---|
| `Movies I` | Stores `Movie` objects in a vector, imports them from CSV with validation and structured exceptions, and prints them as JSON-style objects. Includes two queries, one of which must look up actors in logarithmic time. |
| `Movies II` | Refactors the first part into an abstract `Title` base class with `Movie` and `Series` subclasses, adds a structured `Actor` class with custom stream operators, and moves to a vector of `shared_ptr`. Adds queries 3 to 5. |
| `Movies III` | Adds auxiliary indices built on `std::map`, `std::multimap`, and `std::unordered_multimap`, including a custom hash for `Actor`. Implements queries 6 to 13, using custom sorting functors, `std::function` predicates, and structured bindings. |

### Arithmetic Expressions

| Folder | Description |
|---|---|
| `Arithmetic Expressions I` | Represents integer expressions as a tree built from a class hierarchy (`Node`, `NumberNode`, operation nodes) wrapped by an `Expression` class. Supports postfix and infix printing, with parentheses only where needed, and evaluation. |
| `Expressions II` | Extends the first part with parsing of infix strings using the shunting-yard algorithm. Adds a custom exception hierarchy, leak-free handling of parse and memory errors, and division-by-zero detection. |

### Templates and Containers

| Folder | Description |
|---|---|
| `Matrix` | A header-only `Matrix<Element, Height, Width>` class template backed by a flat `std::array`. Supports `m[row][column]` indexing through an inner helper class, increment and arithmetic operators, and copy-on-write sharing via `shared_ptr`. |
| `Flexible Array I` | A header-only `Array<T>` container whose elements live in fixed-size blocks tracked by a vector, so element addresses never change. Uses low-level allocation and placement new, gives a strong exception guarantee, and implements copy and move semantics. |
| `Flexible Array II` | Finishes the container by adding the `lib` namespace, concepts, extra constructors, `push_back` and `emplace_back` with perfect forwarding, random access iterators (modifiable and constant), `resize`, `erase`, and static assertions. |

### Big Assignments

| Folder | Description |
|---|---|
| `Regular Expressions` | Reads regular expressions from command-line arguments (`-a`) or files (`-f`) and parses them into a syntax tree of `unique_ptr` nodes using an extended shunting-yard algorithm. For each expression it computes the Starting, Neighbors, Ending, and Epsilon functions of the neighbors method used to build finite automata. Errors are reported through a custom exception hierarchy. |
| `Property Graphs` | Class templates for directed and undirected graphs (`DirectedGraph`, `UndirectedGraph`) that attach custom data to nodes and edges. Includes an adjacency matrix, text serialization and import, iterators, the strong exception guarantee, and copy and move semantics. Built on the `lib::Array` flexible array, though `std::vector` is also allowed. |

## Requirements

- A C++20 compiler (concepts, `std::format`, and related features are used in the later projects)
- Visual Studio or any other build setup that can compile each folder as its own project

## Notes

- `Matrix`, `Flexible Array I`, and `Flexible Array II` are header-only because they are templates.
- `Property Graphs` builds on the `Array.h` container from the Flexible Array projects.
