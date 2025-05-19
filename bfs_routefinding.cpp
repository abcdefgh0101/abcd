#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Graph storage
unordered_map<string, vector<string>> graph;

// BFS Route Finder
vector<string> bfs_route(const string& start, const string& goal) {
    queue<string> frontier;
    unordered_map<string, string> came_from;
    
    frontier.push(start);
    came_from[start] = "";
    
    while (!frontier.empty()) {
        string current = frontier.front();
        frontier.pop();
        
        if (current == goal) break;
        
        for (const string& neighbor : graph[current]) {
            if (came_from.find(neighbor) == came_from.end()) {
                frontier.push(neighbor);
                came_from[neighbor] = current;
            }
        }
    }
    
    // Reconstruct path
    vector<string> path;
    string current = goal;
    while (current != "") {
        path.push_back(current);
        current = came_from[current];
    }
    reverse(path.begin(), path.end());
    
    return path;
}

// DFS Route Finder
vector<string> dfs_route(const string& start, const string& goal) {
    stack<string> frontier;
    unordered_map<string, string> came_from;
    
    frontier.push(start);
    came_from[start] = "";
    
    while (!frontier.empty()) {
        string current = frontier.top();
        frontier.pop();
        
        if (current == goal) break;
        
        for (const string& neighbor : graph[current]) {
            if (came_from.find(neighbor) == came_from.end()) {
                frontier.push(neighbor);
                came_from[neighbor] = current;
            }
        }
    }
    
    // Reconstruct path
    vector<string> path;
    string current = goal;
    while (current != "") {
        path.push_back(current);
        current = came_from[current];
    }
    reverse(path.begin(), path.end());
    
    return path;
}

int main() {
    int nodeCount, edgeCount;
    string start, goal;
    
    cout << "=== Simple Route Finder ===\n";
    
    // Input nodes
    cout << "Enter number of vertices: ";
    cin >> nodeCount;
    
    cout << "Enter vertex names (one per line):\n";
    for (int i = 0; i < nodeCount; i++) {
        string name;
        cin >> name;
        graph[name] = {};
    }
    
    // Input edges
    cout << "\nEnter number of edges: ";
    cin >> edgeCount;
    
    cout << "Enter edges (from to, one per line):\n";
    for (int i = 0; i < edgeCount; i++) {
        string from, to;
        cin >> from >> to;
        graph[from].push_back(to);
        graph[to].push_back(from); // Undirected graph
    }
    
    // Input start and goal
    cout << "\nEnter start and goal vertices: ";
    cin >> start >> goal;
    
    // Choose algorithm
    cout << "Choose algorithm (1-BFS, 2-DFS): ";
    int choice;
    cin >> choice;
    
    // Find route
    vector<string> route;
    if (choice == 1) {
        route = bfs_route(start, goal);
    } else {
        route = dfs_route(start, goal);
    }
    
    // Output result
    cout << "\nRoute from " << start << " to " << goal << ":\n";
    if (route.empty() || route[0] != start) {
        cout << "No route found!\n";
    } else {
        for (size_t i = 0; i < route.size(); i++) {
            cout << route[i];
            if (i != route.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    
    return 0;
}