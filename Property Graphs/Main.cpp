#include "Graph.h"

int main() {
    // Create a directed graph with node data as std::string and edge data as int
    try {
        DirectedGraph<std::string, int> graph;
        UndirectedGraph<std::string, int> undirectedGraph;

        // Add nodes
        std::cout << "Adding nodes to graph:" << std::endl;
        auto& nodes = graph.nodes();
        std::string x = "A";

        nodes.add(std::move(x));
        std::cout << std::endl;
        nodes.add("B");
        std::cout << std::endl;
        nodes.add("C");
        std::cout << std::endl;

        auto& undirectednodes = undirectedGraph.nodes();
        undirectednodes.add("A");
        std::cout << std::endl;
        undirectednodes.add("B");
        std::cout << std::endl;
        undirectednodes.add("C");
        std::cout << std::endl;

        // Add edges
        std::cout << "Adding edges to graph:" << std::endl;
        auto& edges = graph.edges();
        edges.add(0, 1, 10); // Edge between Node 0 and Node 1 with weight 10
        edges.add(1, 2, 20); // Edge between Node 1 and Node 2 with weight 20

        auto& undirectedEdges = undirectedGraph.edges();
        undirectedEdges.add(0, 1, 10); // Edge between Node 0 and Node 1 with weight 10
        undirectedEdges.add(1, 2, 20); // Edge between Node 1 and Node 2 with weight 20

        undirectedGraph.edges().printMatrix();
        std::cout << std::endl;

        // testing edge existence methods
        std::cout << "testing undirected graph edge get methods:" << std::endl;
        std::cout << "testing identifiers get method between node 0 and 1: " << std::endl;
        std::cout << undirectedGraph.edges().get(1, 0) << std::endl;
        std::cout << "testing identifiers operator method between node 0 and 1: " << std::endl;
        std::cout << undirectedGraph.edges()[1][0] << std::endl;
        std::cout << std::endl;

        DirectedGraph<std::string, int> copyGraph(graph);
        UndirectedGraph<std::string, int> undirectedCopyGraph(undirectedGraph);

        DirectedGraph<std::string, int> movedGraph = std::move(graph);

        std::cout << "copied graph:" << std::endl;
        std::cout << copyGraph;
        std::cout << std::endl;

        std::cout << "undirected copied graph representation:" << std::endl;
        undirectedCopyGraph.edges().printMatrix();
        std::cout << std::endl;

        std::cout << "undirected source copied graph representation:" << std::endl;
        undirectedGraph.edges().printMatrix();
        std::cout << std::endl;

        std::cout << "moved graph :" << std::endl;
        std::cout << movedGraph;
        std::cout << std::endl;

        std::cout << "moved graph representation :" << std::endl;
        movedGraph.edges().printMatrix();
        std::cout << std::endl;

        std::cout << "testing edges paired get method" << std::endl;
        std::cout << copyGraph.edges().get(0, 1) << std::endl;
        std::cout << copyGraph.edges()[0][1];
        std::cout << std::endl << std::endl;

        DirectedGraph<std::string, int> assignmentMoveGraph;
        assignmentMoveGraph = std::move(copyGraph);

        std::cout << "assignment moved graph :" << std::endl;
        std::cout << assignmentMoveGraph;
        std::cout << std::endl;

        std::cout << "assignment moved graph representation :" << std::endl;
        assignmentMoveGraph.edges().printMatrix();
        std::cout << std::endl << std::endl;

        DirectedGraph<std::string, int> assignmentCopyGraph;
        assignmentCopyGraph = assignmentMoveGraph;

        std::cout << "assignment copied graph representation :" << std::endl;
        assignmentCopyGraph.edges().printMatrix();
        std::cout << std::endl << std::endl;

        std::cout << "Printing to file" << std::endl;
        assignmentCopyGraph.print("Temp-Graph-0172-Output.txt");
    }

    catch (FixedException e) {
        std::cerr << e.message();
    }

    catch (DynamicMessageException e) {
        std::cerr << e.message();
    }

    catch (const std::exception ex) {
        std::cerr << ex.what();
    }

    DirectedGraph<int, std::string> importGraph;

    try {
        std::cout << "importing Graph" << std::endl;
        importGraph.import("input.txt");
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
