#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;

// Structure to represent a node in the graph
struct Node {
    string name;
    int heuristic;  // Estimated cost to goal
    vector<pair<Node*, int>> neighbors; // Connected nodes and edge costs

    Node(string n, int h) : name(n), heuristic(h) {}
};

// Comparator for priority queue (min-heap based on heuristic + path cost)
struct CompareNodes {
    bool operator()(const pair<Node*, int>& a, const pair<Node*, int>& b) {
        return (a.second + a.first->heuristic) > (b.second + b.first->heuristic);
    }
};

class RouteFinder {
private:
    unordered_map<string, Node*> graph;

public:
    // Add a node to the graph
    void addNode(const string& name, int heuristic) {
        graph[name] = new Node(name, heuristic);
    }

    // Add an edge between two nodes
    void addEdge(const string& from, const string& to, int cost) {
        graph[from]->neighbors.push_back({graph[to], cost});
        graph[to]->neighbors.push_back({graph[from], cost}); // Undirected graph
    }

    // Best-First Search implementation
    vector<string> findRoute(const string& start, const string& goal) {
        priority_queue<pair<Node*, int>, vector<pair<Node*, int>>, CompareNodes> frontier;
        unordered_map<Node*, Node*> came_from;
        unordered_map<Node*, int> cost_so_far;

        Node* startNode = graph[start];
        Node* goalNode = graph[goal];

        frontier.push({startNode, 0});
        came_from[startNode] = nullptr;
        cost_so_far[startNode] = 0;

        while (!frontier.empty()) {
            auto current = frontier.top().first;
            int current_cost = frontier.top().second;
            frontier.pop();

            if (current == goalNode) {
                break; // Found the goal
            }

            for (auto& neighbor : current->neighbors) {
                Node* next = neighbor.first;
                int new_cost = current_cost + neighbor.second;

                if (came_from.find(next) == came_from.end() || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    came_from[next] = current;
                    frontier.push({next, new_cost});
                }
            }
        }

        // Reconstruct the path
        vector<string> path;
        for (Node* current = goalNode; current != nullptr; current = came_from[current]) {
            path.push_back(current->name);
        }
        reverse(path.begin(), path.end());

        return path;
    }

    // Clean up memory
    ~RouteFinder() {
        for (auto& pair : graph) {
            delete pair.second;
        }
    }
};

int main() {
    RouteFinder finder;

    cout << "=== Route Finder using Best-First Search ===" << endl;
    
    // Get nodes from user
    int nodeCount;
    cout << "Enter number of locations: ";
    cin >> nodeCount;

    for (int i = 0; i < nodeCount; i++) {
        string name;
        int heuristic;
        cout << "Enter location " << i+1 << " name: ";
        cin >> name;
        cout << "Enter heuristic value for " << name << ": ";
        cin >> heuristic;
        finder.addNode(name, heuristic);
    }

    // Get edges from user
    int edgeCount;
    cout << "Enter number of connections between locations: ";
    cin >> edgeCount;

    for (int i = 0; i < edgeCount; i++) {
        string from, to;
        int cost;
        cout << "enter from, to, cost" <<endl;
        cin >> from >> to >> cost;;
        
        finder.addEdge(from, to, cost);
    }

    // Get start and goal
    string start, goal;
    cout << "Enter starting location: ";
    cin >> start;
    cout << "Enter destination: ";
    cin >> goal;

    // Find and display route
    vector<string> route = finder.findRoute(start, goal);

    cout << "\nOptimal Route from " << start << " to " << goal << ":\n";
    if (route.size() == 1 && route[0] != start) {
        cout << "No valid route found!" << endl;
    } else {
        for (size_t i = 0; i < route.size(); i++) {
            cout << route[i];
            if (i != route.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    return 0;
}