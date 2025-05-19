#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional> 

using namespace std;

class GraphColoring {
private:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list
    vector<int> colors; // Color assignments
    int numColors; // Available colors (1 to numColors)
    map<string, function<bool(int, int)>> constraints; // Store constraints

public:
    GraphColoring(int vertices, int colors) : V(vertices), numColors(colors) {
        adj.resize(V);
        this->colors.resize(V, 0); // 0 means uncolored
        initializeConstraints();
    }

    // Add edge between vertices u and v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Initialize all constraints
    void initializeConstraints() {
        // Basic constraint: adjacent nodes must have different colors
        constraints["adjacent_diff"] = [this](int node, int color) {
            for (int neighbor : adj[node]) {
                if (colors[neighbor] == color) {
                    return false;
                }
            }
            return true;
        };
    }

    // Check if color assignment is valid for all constraints
    bool isSafe(int node, int color) {
        for (const auto& constraint : constraints) {
            if (!constraint.second(node, color)) {
                return false;
            }
        }
        return true;
    }

    // Recursive backtracking function
    bool graphColoringUtil(int node) {
        if (node == V) {
            return true; // All nodes colored
        }

        for (int c = 1; c <= numColors; c++) {
            if (isSafe(node, c)) {
                colors[node] = c;

                if (graphColoringUtil(node + 1)) {
                    return true;
                }

                colors[node] = 0; // Backtrack
            }
        }
        return false;
    }

    // Main coloring function
    bool colorGraph() {
        if (!graphColoringUtil(0)) {
            cout << "Solution does not exist with " << numColors << " colors." << endl;
            return false;
        }

        printSolution();
        return true;
    }

    // Print the coloring solution
    void printSolution() {
    vector<string> colorNames = {"Red", "Green", "Blue", "Yellow", "Purple", "Orange", 
                                "Cyan", "Magenta", "Lime", "Pink"}; // Extend as needed
    
    cout << "\nGraph coloring solution:" << endl;
    for (int i = 0; i < V; i++) {
        // Check bounds to prevent out-of-range access
        if (colors[i] >= 0 && colors[i] < colorNames.size()) {
            cout << "Vertex " << i << " -> Color " << colorNames[colors[i]] << endl;
        } else {
            cout << "Vertex " << i << " -> Invalid color index: " << colors[i] << endl;
        }
    }
}
};

int main() {
    int numNodes, numEdges, numColors;

    cout << "=== Graph Coloring Problem ===" << endl;
    cout << "Enter number of nodes: ";
    cin >> numNodes;
    cout << "Enter number of edges: ";
    cin >> numEdges;
    cout << "Enter number of colors available: ";
    cin >> numColors;

    GraphColoring gc(numNodes, numColors);

    cout << "\nEnter edges (pair of nodes, 0-based index):" << endl;
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        cout << "Edge " << i+1 << ": ";
        cin >> u >> v;
        gc.addEdge(u, v);
    }

    // Solve the graph coloring problem
    cout << "\nAttempting to color graph with " << numColors << " colors..." << endl;
    gc.colorGraph();

    return 0;
}