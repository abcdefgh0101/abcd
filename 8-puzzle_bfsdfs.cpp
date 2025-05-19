#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
using namespace std;

// Structure to represent a state of the puzzle
struct PuzzleState {
    vector<vector<int>> board;
    int zero_row, zero_col; // Position of the empty tile (0)
    string path; // Path taken to reach this state
    
    // Constructor
    PuzzleState(vector<vector<int>> b, int zr, int zc, string p) 
        : board(b), zero_row(zr), zero_col(zc), path(p) {}
};

// Function to check if two states are equal
bool areStatesEqual(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to convert board to a string for hashing
string boardToString(const vector<vector<int>>& board) {
    string s;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            s += to_string(board[i][j]);
        }
    }
    return s;
}

// Function to check if the current state is the goal state
bool isGoalState(const vector<vector<int>>& board) {
    vector<vector<int>> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    return areStatesEqual(board, goal);
}

// Function to print the board
void printBoard(const vector<vector<int>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to generate possible moves
vector<PuzzleState> generateMoves(const PuzzleState& state) {
    vector<PuzzleState> moves;
    int row = state.zero_row;
    int col = state.zero_col;
    
    // Directions: up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    string directions[] = {"U", "D", "L", "R"};
    
    for (int i = 0; i < 4; i++) {
        int new_row = row + dr[i];
        int new_col = col + dc[i];
        
        if (new_row >= 0 && new_row < 3 && new_col >= 0 && new_col < 3) {
            vector<vector<int>> new_board = state.board;
            swap(new_board[row][col], new_board[new_row][new_col]);
            
            string new_path = state.path + directions[i];
            moves.push_back(PuzzleState(new_board, new_row, new_col, new_path));
        }
    }
    
    return moves;
}

// Breadth-First Search implementation
void bfs(const vector<vector<int>>& initial) {
    queue<PuzzleState> q;
    unordered_set<string> visited;
    
    // Find initial zero position
    int zero_row, zero_col;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (initial[i][j] == 0) {
                zero_row = i;
                zero_col = j;
                break;
            }
        }
    }
    
    PuzzleState start(initial, zero_row, zero_col, "");
    q.push(start);
    visited.insert(boardToString(initial));
    
    int nodes_expanded = 0;
    
    while (!q.empty()) {
        PuzzleState current = q.front();
        q.pop();
        nodes_expanded++;
        
        if (isGoalState(current.board)) {
            cout << "\nSolution found!" << endl;
            cout << "Path: " << current.path << endl;
            cout << "Number of moves: " << current.path.length() << endl;
            cout << "Nodes expanded: " << nodes_expanded << endl;
            return;
        }
        
        vector<PuzzleState> moves = generateMoves(current);
        for (const PuzzleState& move : moves) {
            string move_str = boardToString(move.board);
            if (visited.find(move_str) == visited.end()) {
                visited.insert(move_str);
                q.push(move);
            }
        }
    }
    
    cout << "\nNo solution found!" << endl;
}

// Depth-First Search implementation
void dfs(const vector<vector<int>>& initial) {
    stack<PuzzleState> s;
    unordered_set<string> visited;
    
    // Find initial zero position
    int zero_row, zero_col;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (initial[i][j] == 0) {
                zero_row = i;
                zero_col = j;
                break;
            }
        }
    }
    
    PuzzleState start(initial, zero_row, zero_col, "");
    s.push(start);
    visited.insert(boardToString(initial));
    
    int nodes_expanded = 0;
    int max_depth = 0;
    
    while (!s.empty()) {
        PuzzleState current = s.top();
        s.pop();
        nodes_expanded++;
        
        if (isGoalState(current.board)) {
            cout << "\nSolution found!" << endl;
            cout << "Path: " << current.path << endl;
            cout << "Number of moves: " << current.path.length() << endl;
            cout << "Nodes expanded: " << nodes_expanded << endl;
            return;
        }
        
        // Limit depth to prevent infinite recursion (optional)
        if (current.path.length() > 50) {
            continue;
        }
        
        vector<PuzzleState> moves = generateMoves(current);
        for (const PuzzleState& move : moves) {
            string move_str = boardToString(move.board);
            if (visited.find(move_str) == visited.end()) {
                visited.insert(move_str);
                s.push(move);
            }
        }
    }
    
    cout << "\nNo solution found!" << endl;
}

int main() {
    cout << "8-Puzzle Solver" << endl;
    cout << "Enter the initial state of the puzzle (3x3 matrix, use 0 for empty tile):" << endl;
    
    vector<vector<int>> initial(3, vector<int>(3));
    
    // Read input from user
    for (int i = 0; i < 3; i++) {
        cout << "Row " << i+1 << ": ";
        for (int j = 0; j < 3; j++) {
            cin >> initial[i][j];
        }
    }
    
    cout << "\nInitial state:" << endl;
    printBoard(initial);
    
    int choice;
    cout << "\nChoose search algorithm:" << endl;
    cout << "1. Breadth-First Search (BFS)" << endl;
    cout << "2. Depth-First Search (DFS)" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            cout << "\nSolving using BFS..." << endl;
            bfs(initial);
            break;
        case 2:
            cout << "\nSolving using DFS..." << endl;
            dfs(initial);
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
    
    return 0;
}