#include <bits/stdc++.h>
using namespace std;

void printPath(map<pair<int, int>, pair<int, int>>& parent, pair<int, int> state) {
    vector<pair<int, int>> path;
    
    while (state != make_pair(0, 0)) {
        path.push_back(state);
        state = parent[state];
    }
    
    path.push_back({0, 0}); 
    reverse(path.begin(), path.end()); 
    cout << "Steps to reach goal:\n";
    for (auto& p : path) {
        cout << "(" << p.first << ", " << p.second << ")\n";
    }
}

bool dfs(int x, int y, int j1, int j2, int goal, set<pair<int, int>>& visited, 
         map<pair<int, int>, pair<int, int>>& parent) {
    
    if (x == goal || y == goal) {
        cout << "Goal reached!\n";
        printPath(parent, {x, y});
        return true;
    }

    visited.insert({x, y});

    vector<pair<int, int>> nextStates = {
        {j1, y},  
        {x, j2},   
        {0, y},    
        {x, 0},    
        {x - min(x, j2 - y), y + min(x, j2 - y)}, 
        {x + min(y, j1 - x), y - min(y, j1 - x)} 
    };

    for (auto state : nextStates) {
        if (visited.find(state) == visited.end()) { 
            parent[state] = {x, y}; 
            if (dfs(state.first, state.second, j1, j2, goal, visited, parent)) {
                return true; 
            }
        }
    }

    return false;
}

void solveWaterJugDFS(int j1, int j2, int goal) {
    set<pair<int, int>> visited;
    map<pair<int, int>, pair<int, int>> parent;
    parent[{0, 0}] = {-1, -1};

    if (!dfs(0, 0, j1, j2, goal, visited, parent)) {
        cout << "Goal cannot be reached.\n";
    }
}

int main() {
    int j1 = 3, j2 = 4, goal = 2;
    solveWaterJugDFS(j1, j2, goal);
    return 0;
}
