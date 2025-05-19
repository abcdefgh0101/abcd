#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <sstream>

using namespace std;

class GraphNode {
private:
    string name;
    int heuristic;
    vector<pair<GraphNode*, int>> neighbors; // Stores neighbor and direct distance

public:
    GraphNode(const string& n, int h) : name(n), heuristic(h) {}

    string getName() const { return name; }
    int getHeuristic() const { return heuristic; }
    const vector<pair<GraphNode*, int>>& getNeighbors() const { return neighbors; }

    void addNeighbor(GraphNode* node, int distance) {
        neighbors.emplace_back(node, distance);
    }
};

class AStarRouteFinder {
private:
    unordered_map<string, GraphNode*> graph;
    
    // Heuristic function now uses stored direct distances
    int calculateHeuristic(GraphNode* current, GraphNode* goal) {
        // Find direct distance if it exists
        for (const auto& neighbor : current->getNeighbors()) {
            if (neighbor.first == goal) {
                return neighbor.second;
            }
        }
        // Fallback to the node's heuristic value if no direct connection
        return current->getHeuristic();
    }

    bool satisfiesCostConstraint(int total_cost) {
        const int MAX_ALLOWED_COST = 1000;
        return total_cost <= MAX_ALLOWED_COST;
    }

    bool isNodeAccessible(GraphNode* node) {
        vector<string> closed_nodes;
        return find(closed_nodes.begin(), closed_nodes.end(), node->getName()) == closed_nodes.end();
    }

public:
    ~AStarRouteFinder() {
        for (auto& pair : graph) {
            delete pair.second;
        }
    }

    void addNode(const string& name, int heuristic) {
        graph[name] = new GraphNode(name, heuristic);
    }

    void addEdge(const string& from, const string& to, int distance) {
        graph[from]->addNeighbor(graph[to], distance);
        graph[to]->addNeighbor(graph[from], distance);
    }

    vector<string> findRoute(const string& start, const string& goal) {
        auto compare = [](const pair<int, pair<int, GraphNode*>>& a, 
                         const pair<int, pair<int, GraphNode*>>& b) {
            return a.first > b.first;
        };

        priority_queue<pair<int, pair<int, GraphNode*>>, 
                      vector<pair<int, pair<int, GraphNode*>>>,
                      decltype(compare)> frontier(compare);

        unordered_map<GraphNode*, GraphNode*> came_from;
        unordered_map<GraphNode*, int> cost_so_far;
        GraphNode* start_node = graph[start];
        GraphNode* goal_node = graph[goal];

        frontier.emplace(0, make_pair(0, start_node));
        came_from[start_node] = nullptr;
        cost_so_far[start_node] = 0;

        while (!frontier.empty()) {
            auto current = frontier.top().second.second;
            int current_cost = frontier.top().second.first;
            frontier.pop();

            if (current == goal_node) {
                break;
            }

            for (const auto& neighbor : current->getNeighbors()) {
                GraphNode* next = neighbor.first;
                int new_cost = current_cost + neighbor.second;

                if (!isNodeAccessible(next) || !satisfiesCostConstraint(new_cost)) {
                    continue;
                }

                if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    int priority = new_cost + calculateHeuristic(next, goal_node);
                    frontier.emplace(priority, make_pair(new_cost, next));
                    came_from[next] = current;
                }
            }
        }

        vector<string> path;
        for (GraphNode* current = goal_node; current != nullptr; current = came_from[current]) {
            path.push_back(current->getName());
        }
        reverse(path.begin(), path.end());

        return path;
    }
};

int main() {
    AStarRouteFinder routeFinder;
    vector<string> nodeNames;
    int nodeCount, edgeCount;

    cout << "=== A* Route Finder ===" << endl;
    
    // Input nodes
    cout << "Enter number of vertices: ";
    cin >> nodeCount;
    cin.ignore(); // Clear newline

    for (int i = 0; i < nodeCount; i++) {
        string name;
        int heuristic;

        cout << "\nEnter vertex " << i+1 << " details:" << endl;
        cout << "Name: ";
        getline(cin, name);
        cout << "Heuristic value (estimated distance to goal): ";
        cin >> heuristic;
        cin.ignore(); // Clear newline

        routeFinder.addNode(name, heuristic);
        nodeNames.push_back(name);
    }

    // Input edges with direct distances
    cout << "\nEnter number of edges: ";
    cin >> edgeCount;
    cin.ignore();

    for (int i = 0; i < edgeCount; i++) {
        string from, to;
        int distance;

        cout << "\nEnter edge " << i+1 << " details:" << endl;
        cout << "From vertex: ";
        getline(cin, from);
        cout << "To vertex: ";
        getline(cin, to);
        cout << "Direct distance between nodes: ";
        cin >> distance;
        cin.ignore();

        routeFinder.addEdge(from, to, distance);
    }

    // Input start and goal
    string start, goal;
    cout << "\nEnter start vertex: ";
    getline(cin, start);
    cout << "Enter goal vertex: ";
    getline(cin, goal);

    // Find and display route
    vector<string> route = routeFinder.findRoute(start, goal);

    cout << "\nOptimal Route from " << start << " to " << goal << ":\n";
    if (route.empty() || route[0] != start) {
        cout << "No valid route found!" << endl;
    } else {
        for (size_t i = 0; i < route.size(); i++) {
            cout << route[i];
            if (i != route.size() - 1) cout << " -> ";
        }
        cout << endl;
    }

    return 0;
}