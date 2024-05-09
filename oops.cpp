#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    string src;
    string dest;
    int weight;

    Edge(string src, string dest, int weight) : src(src), dest(dest), weight(weight) {}
};

// Function to compare edges based on their weights (for sorting)
bool compareEdges(const Edge& edge1, const Edge& edge2) {
    return edge1.weight < edge2.weight;
}

// Function to find the parent of a vertex (used in the disjoint set)
int findParent(const vector<int>& parent, int vertex) {
    if (parent[vertex] == vertex)
        return vertex;
    return findParent(parent, parent[vertex]);
}

// Function to perform union of two sets (used in the disjoint set)
void unionSets(vector<int>& parent, vector<int>& rank, int vertex1, int vertex2) {
    int root1 = findParent(parent, vertex1);
    int root2 = findParent(parent, vertex2);
    if (rank[root1] < rank[root2])
        parent[root1] = root2;
    else if (rank[root1] > rank[root2])
        parent[root2] = root1;
    else {
        parent[root2] = root1;
        rank[root1]++;
    }
}

// Function to construct and print the minimum spanning tree using Kruskal's algorithm
void constructMinimumSpanningTree(const vector<Edge>& edges, const unordered_map<string, int>& vertexMap) {
    int numVertices = vertexMap.size();
    vector<int> parent(numVertices);
    vector<int> rank(numVertices, 0);

    // Initialize parent array for the disjoint set
    for (const auto& vertex : vertexMap) {
        parent[vertex.second] = vertex.second;
    }

    vector<Edge> minimumSpanningTree;
    for (const auto& edge : edges) {
        int srcIndex = vertexMap.at(edge.src);
        int destIndex = vertexMap.at(edge.dest);

        int parent1 = findParent(parent, srcIndex);
        int parent2 = findParent(parent, destIndex);

        // Include the edge in the minimum spanning tree if it doesn't form a cycle
        if (parent1 != parent2) {
            minimumSpanningTree.push_back(edge);
            unionSets(parent, rank, parent1, parent2);
        }
    }

    cout << "Minimum Spanning Tree Path:" << endl;
    int totalDistance = 0;
    for (const auto& edge : minimumSpanningTree) {
        cout << edge.src << " -- " << edge.dest << " : " << edge.weight << endl;
        totalDistance += edge.weight;
    }
    cout << "Total distance of the minimum spanning tree: " << totalDistance << endl;

    // Generate graph visualization using Graphviz
    ofstream dotFile("minimum_spanning_tree.dot");
    if (dotFile.is_open()) {
        dotFile << "graph MinimumSpanningTree {\n";
        dotFile << "node [shape=circle]\n";

        // Write vertices
        for (const auto& vertex : vertexMap) {
            dotFile << "  " << vertex.second << " [label=\"" << vertex.first << "\"]\n";
        }

        // Write edges
        for (const auto& edge : minimumSpanningTree) {
            dotFile << "  " << vertexMap.at(edge.src) << " -- " << vertexMap.at(edge.dest) << " [label=\"" << edge.weight << "\"]\n";
        }

        dotFile << "}";
        dotFile.close();

        // Generate the visualization using Graphviz
        system("dot -Tpng minimum_spanning_tree.dot -o minimum_spanning_tree.png");
        cout << "Graph visualization generated: minimum_spanning_tree.png" << endl;
    }
}

int main() {
    int numVertices, numEdges;
    cout << "Enter the number of Places: ";
    cin >> numVertices;

    vector<Edge> edges;
    unordered_map<string, int> vertexMap; // Map to store vertex names and their corresponding indices

    cout << "Enter the names of Places:" << endl;
    for (int i = 0; i < numVertices; i++) {
        string vertexName;
        cout << "Enter name for Place " << i + 1 << ": ";
        cin >> vertexName;

        vertexMap[vertexName] = i;
    }

    cout << "Enter the number of routes: ";
    cin >> numEdges;

    cout << "Enter the Source, Destination and Distance:" << endl;
    for (int i = 0; i < numEdges; i++) {
        string src, dest;
        int weight;
        cout << "Enter Route " << i + 1 << " source, destination, and weight: ";
        cin >> src >> dest >> weight;

        edges.emplace_back(src, dest, weight);
    }
    ofstream inputDotFile("input_graph.dot");
if (inputDotFile.is_open()) {
    inputDotFile << "graph InputGraph {\n";
    inputDotFile << "node [shape=circle]\n";

    // Write vertices
    for (const auto& vertex : vertexMap) {
        inputDotFile << "  " << vertex.second << " [label=\"" << vertex.first << "\"]\n";
    }

    // Write edges
    for (const auto& edge : edges) {
        inputDotFile << "  " << vertexMap.at(edge.src) << " -- " << vertexMap.at(edge.dest) << " [label=\"" << edge.weight << "\"]\n";
    }

    inputDotFile << "}";
    inputDotFile.close();

    // Generate the visualization using Graphviz
    system("dot -Tpng input_graph.dot -o input_graph.png");
    cout << "Input graph visualization generated: input_graph.png" << endl;
}

    // Sort the edges based on their weights
    sort(edges.begin(), edges.end(), compareEdges);

    constructMinimumSpanningTree(edges, vertexMap);

    return 0;
}
