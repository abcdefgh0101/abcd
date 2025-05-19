#include <bits/stdc++.h>
using namespace std;

void bfs(int j1, int j2, int goal) {
    queue<pair<pair<int, int>, int>> q;
    set<pair<int, int>> visited;
    
    q.push({{0, 0}, 0});
    visited.insert({0, 0});

    while (!q.empty()) {
        int x = q.front().first.first;   
        int y = q.front().first.second;  
        int steps = q.front().second;
        q.pop();

        if (x == goal || y == goal) {
            cout << "Goal reached in " << steps << " steps!\n";
            return;
        }

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
                q.push({state, steps + 1});
                visited.insert(state);
            }
        }
    }

    cout << "Goal cannot be reached.\n";
}

int main() {
    int j1 = 3, j2 = 4, goal = 2;
    bfs(j1, j2, goal);
    return 0;
}
