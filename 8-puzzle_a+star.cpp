#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

// Possible moves (Up, Down, Left, Right)
const int dr[] = {-1, 1, 0, 0};  // Row changes
const int dc[] = {0, 0, -1, 1};   // Column changes
const char moves[] = {'U', 'D', 'L', 'R'};

// Goal state
const vector<vector<int>> GOAL = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

// Function to convert board to string for hashing
string boardToString(const vector<vector<int>>& board) {
    string key;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            key += to_string(board[i][j]);
        }
    }
    return key;
}

// Calculate Manhattan distance heuristic
int calculateHeuristic(const vector<vector<int>>& board) {
    int distance = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = board[i][j];
            if (num != 0) {
                int goal_row = (num - 1) / 3;
                int goal_col = (num - 1) % 3;
                distance += abs(i - goal_row) + abs(j - goal_col);
            }
        }
    }
    return distance;
}

// Function to solve the 8-puzzle
string solvePuzzle(vector<vector<int>> start) {
    // Priority queue: (total_cost, g_cost, board, zero_row, zero_col, path)
    priority_queue<pair<int, pair<int, pair<vector<vector<int>>, pair<int, pair<int, string>>>>>, 
                   vector<pair<int, pair<int, pair<vector<vector<int>>, pair<int, pair<int, string>>>>>>,
                   greater<pair<int, pair<int, pair<vector<vector<int>>, pair<int, pair<int, string>>>>>>> pq;
    
    unordered_map<string, int> visited;
    
    // Find initial position of 0
    int zero_r, zero_c;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (start[i][j] == 0) {
                zero_r = i;
                zero_c = j;
                break;
            }
        }
    }
    
    // Push initial state
    int h = calculateHeuristic(start);
    pq.push({h, {0, {start, {zero_r, {zero_c, ""}}}}});
    visited[boardToString(start)] = h;
    
    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        
        int current_cost = current.first;
        int g = current.second.first;
        vector<vector<int>> board = current.second.second.first;
        int r = current.second.second.second.first;
        int c = current.second.second.second.second.first;
        string path = current.second.second.second.second.second;
        
        // Check if goal reached
        if (board == GOAL) {
            return path;
        }
        
        // Generate all possible moves
        for (int i = 0; i < 4; i++) {
            int new_r = r + dr[i];
            int new_c = c + dc[i];
            
            // Check if move is valid
            if (new_r >= 0 && new_r < 3 && new_c >= 0 && new_c < 3) {
                // Create new board state
                vector<vector<int>> new_board = board;
                swap(new_board[r][c], new_board[new_r][new_c]);
                
                string key = boardToString(new_board);
                int new_g = g + 1;
                int new_h = calculateHeuristic(new_board);
                int new_cost = new_g + new_h;
                
                // Check if we've seen this state with higher cost
                if (visited.find(key) == visited.end() || new_cost < visited[key]) {
                    visited[key] = new_cost;
                    pq.push({new_cost, {new_g, {new_board, {new_r, {new_c, path + moves[i]}}}}});
                }
            }
        }
    }
    
    return "No solution";
}

int main() {
    cout << "Enter the initial puzzle state (3x3 matrix, 0 for empty tile):\n";
    vector<vector<int>> start(3, vector<int>(3));
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> start[i][j];
        }
    }
    
    string solution = solvePuzzle(start);
    
    if (solution == "No solution") {
        cout << "No solution exists for this configuration.\n";
    } else {
        cout << "Solution found in " << solution.size() << " moves: " << solution << "\n";
    }
    
    return 0;
}