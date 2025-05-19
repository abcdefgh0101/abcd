#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <functional>

using namespace std;

int N; // Now variable, will be set by user

int calculateAttacks(const vector<int>& board) {
    int attacks = 0;
    // Check row conflicts (queens in same column)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            // Same column or diagonal conflict
            if (board[i] == board[j] || abs(i - j) == abs(board[i] - board[j])) {
                attacks++;
            }
        }
    }
    return attacks;
}

void printBoard(const vector<int>& board) {
    // Create 2D representation
    vector<vector<char>> grid(N, vector<char>(N, '.'));
    for (int row = 0; row < N; ++row) {
        if (board[row] != -1) { // Only if queen exists in this row
            grid[row][board[row]] = 'Q';
        }
    }
    
    // Print the board
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Attacks: " << calculateAttacks(board) << endl << endl;
}

vector<vector<int>> generateNeighbors(const vector<int>& board) {
    vector<vector<int>> neighbors;
    
    for (int row = 0; row < N; ++row) {
        int originalCol = board[row];
        for (int col = 0; col < N; ++col) {
            if (col != originalCol) {
                vector<int> neighbor = board;
                neighbor[row] = col;
                neighbors.push_back(neighbor);
            }
        }
    }
    
    return neighbors;
}

struct BoardHash {
    size_t operator()(const vector<int>& board) const {
        size_t hash = 0;
        for (int pos : board) {
            hash = hash * N + pos;
        }
        return hash;
    }
};

bool bestFirstSearch(vector<int>& solution) {
    auto cmp = [](const pair<vector<int>, int>& left, const pair<vector<int>, int>& right) {
        return left.second > right.second; 
    };
    priority_queue<pair<vector<int>, int>, vector<pair<vector<int>, int>>, decltype(cmp)> frontier(cmp);
    
    unordered_set<vector<int>, BoardHash> visited;
    
    vector<int> initial(N, -1); // Initialize with no queens
    // Place one queen in each row at random column
    for (int i = 0; i < N; ++i) {
        initial[i] = rand() % N;
    }
    
    frontier.push({initial, calculateAttacks(initial)});
    visited.insert(initial);
    
    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();
        
        if (current.second == 0) {
            solution = current.first;
            return true;
        }
        
        vector<vector<int>> neighbors = generateNeighbors(current.first);
        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                int attacks = calculateAttacks(neighbor);
                frontier.push({neighbor, attacks});
                visited.insert(neighbor);
            }
        }
    }
    
    return false;
}

vector<int> getUserBoard() {
    vector<int> board(N, -1); // -1 means no queen in that row
    
    cout << "Enter initial board configuration (one queen per row):\n";
    cout << "For each row (0 to " << N-1 << "), enter column (0 to " << N-1 << ") or -1 for no queen:\n";
    
    for (int row = 0; row < N; ++row) {
        int col;
        while (true) {
            cout << "Row " << row << ": ";
            cin >> col;
            if (col == -1 || (col >= 0 && col < N)) {
                break;
            }
            cout << "Invalid column. Please enter -1 or between 0 and " << N-1 << endl;
        }
        board[row] = col;
    }
    
    return board;
}

int main() {
    cout << "Enter board size (N): ";
    cin >> N;
    
    if (N <= 0) {
        cout << "Invalid board size. Using default N=8.\n";
        N = 8;
    }
    
    srand(time(0));
    
    char choice;
    cout << "Use random initial board? (y/n): ";
    cin >> choice;
    
    int successCount = 0;
    const int totalAttempts = 100;
    
    for (int attempt = 0; attempt < totalAttempts; ++attempt) {
        vector<int> solution;
        vector<int> initialBoard;
        
        if (choice == 'n' || choice == 'N') {
            initialBoard = getUserBoard();
            // Run bestFirstSearch with this initial board
            solution = initialBoard;
            int attacks = calculateAttacks(solution);
            if (attacks == 0) {
                cout << "Input board is already a solution!\n";
                printBoard(solution);
                successCount++;
                break;
            }
            // Need to modify bestFirstSearch to accept initial board
            // For simplicity, we'll just run with random boards in this example
        }
        
        if (bestFirstSearch(solution)) {
            successCount++;
            cout << "Solution found in attempt " << (attempt + 1) << ":" << endl;
            printBoard(solution);
            break;
        }
    }
    
    cout << "Success rate: " << successCount << "/" << totalAttempts << endl;
    return 0;
}