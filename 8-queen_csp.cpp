#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class NQueensCSP {
private:
    int N;
    vector<int> queens;            
    vector<set<int>> domains;        

public:
    NQueensCSP(int n) {
        N = n;
        queens.assign(N, -1);
        domains.resize(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                domains[i].insert(j);   
            }
        }
    }

    bool solve() {
        return solveCSP();
    }

    void printSolution() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (queens[i] == j)
                    cout << "Q ";
                else
                    cout << ". ";
            }
            cout << endl;
        }
    }

private:
bool solveCSP() {
    if (allAssigned())
        return true;

    int row = selectRowMRV();

    vector<int> domainList(domains[row].begin(), domains[row].end()); 
    for (int col : domainList) {
        if (isSafe(row, col)) {
            queens[row] = col;
            vector<set<int>> oldDomains = domains; 
            if (forwardCheck(row, col)) {          
                if (solveCSP())
                    return true;
            }

            domains = oldDomains;
            queens[row] = -1;
        }
    }

    return false;
}


    bool allAssigned() {
        for (int i = 0; i < N; i++) {
            if (queens[i] == -1)
                return false;
        }
        return true;
    }

    int selectRowMRV() {
        int minOptions = N + 1;
        int selectedRow = -1;
        for (int i = 0; i < N; i++) {
            if (queens[i] == -1) {
                int options = domains[i].size();
                if (options < minOptions) {
                    minOptions = options;
                    selectedRow = i;
                }
            }
        }
        return selectedRow;
    }

    bool isSafe(int row, int col) {
        for (int i = 0; i < row; i++) {
            if (queens[i] == col)
                return false;
            if (abs(queens[i] - col) == abs(i - row))
                return false;
        }
        return true;
    }

    bool forwardCheck(int row, int col) {
        for (int r = row + 1; r < N; r++) {
            domains[r].erase(col);               
            domains[r].erase(col + (r - row));    
            domains[r].erase(col - (r - row));    
            
            if (domains[r].empty()) {             
                return false;
            }
        }
        return true;
    }
    
};

int main() {
    int N;
    cout << "Enter value of N: ";
    cin >> N;

    NQueensCSP solver(N);

    if (solver.solve()) {
        cout << "Solution found:\n";
        solver.printSolution();
    } else {
        cout << "No solution exists.\n";
    }

    return 0;
}





// domain = stores available position for queen to be placed 
// modified after every placement = forward check
// mrv = minimun remaining value - check the smallest size domain and return that particualr row
