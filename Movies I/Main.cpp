#include <vector>
#include "Movie.h"
#include "Database.h"
#include "Queries.h"

int main()
{
    std::vector<Movie> db;

    // Manually add a couple of sample movies
    db.emplace_back(Movie("Bobule", 2008, "comedy", 65, { "Krystof Hadek", "Tereza Voriskova" }));
    db.emplace_back(Movie("Inception", 2010, "science fiction", 87, {}));
    db.emplace_back(Movie("The Godfather", 1972, "crime", 98, { "Marlon Brando", "Al Pacino" }));

    try {
        Database::import("Input.txt", db);
    }
    catch (const Exception& e) {
        std::cerr << "Error: " << e.text << " (Code: " << e.code << ")" << std::endl;
        return e.code;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return -1;
    }

    for (Movie movie : db) {
        movie.print_json();
        std::cout << std::endl;
    }

    db_query_2(db);


    return 0;
}