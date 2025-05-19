#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

class BoardState {
public:
    vector<int> queens; // queens[i] = column of queen in row i
    int g; // number of queens placed (depth)
    int h; // heuristic (conflicts)

    BoardState(vector<int> q, int g_cost) : queens(q), g(g_cost) {
        h = computeHeuristic();
    }

    int computeHeuristic() {
        int conflicts = 0;
        for (int i = 0; i < queens.size(); ++i) {
            for (int j = i + 1; j < queens.size(); ++j) {
                if (queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j)) {
                    conflicts++;
                }
            }
        }
        return conflicts;
    }

    int f() const {
        return g + h;
    }

    // for min-heap in priority_queue
    bool operator<(const BoardState& other) const {
        return f() > other.f(); // reverse for min-heap
    }
};

class NQueensSolver {
private:
    int n;

    bool isSafe(const vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; ++i) {
            if (queens[i] == col || abs(queens[i] - col) == abs(i - row))
                return false;
        }
        return true;
    }

    void printBoard(const vector<int>& queens) {
        for (int row = 0; row < queens.size(); ++row) {
            for (int col = 0; col < n; ++col) {
                cout << (queens[row] == col ? "Q " : ". ");
            }
            cout << endl;
        }
    }

public:
    NQueensSolver(int size) : n(size) {}

    bool solve() {
        priority_queue<BoardState> pq;
        pq.push(BoardState({}, 0));

        while (!pq.empty()) {
            BoardState current = pq.top();
            pq.pop();

            if (current.queens.size() == n && current.h == 0) {
                printBoard(current.queens);
                return true;
            }

            int row = current.queens.size();
            for (int col = 0; col < n; ++col) {
                if (isSafe(current.queens, row, col)) {
                    vector<int> next_queens = current.queens;
                    next_queens.push_back(col);
                    pq.push(BoardState(next_queens, current.g + 1));
                }
            }
        }

        return false;
    }
};

int main() {
    int n;
    cout << "Enter number of queens: ";
    cin >> n;
    NQueensSolver solver(n);
    if (!solver.solve()) {
        cout << "No solution found." << endl;
    }
    return 0;
}
