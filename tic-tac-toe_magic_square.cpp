#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int magicSquare[3][3] = {
    {8, 1, 6},
    {3, 5, 7},
    {4, 9, 2}
};

void displayBoard(char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "---------\n";
    }
    cout << "\n";
}

bool checkWin(const vector<int>& moves) {
    int n = moves.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                if (moves[i] + moves[j] + moves[k] == 15) {
                    return true;
                }
            }
        }
    }
    return false;
}

pair<int, int> findBestMove(const vector<int>& playerMoves, const vector<int>& computerMoves, const vector<pair<int, int>>& availableMoves) {

    vector<int> allMoves;
    for (const auto& move : playerMoves) allMoves.push_back(move);
    for (const auto& move : computerMoves) allMoves.push_back(move);

    for (const auto& move : availableMoves) {
        int magicValue = magicSquare[move.first][move.second];
        vector<int> testMoves = computerMoves;
        testMoves.push_back(magicValue);
        if (checkWin(testMoves)) return move;
    }

    for (const auto& move : availableMoves) {
        int magicValue = magicSquare[move.first][move.second];
        vector<int> testMoves = playerMoves;
        testMoves.push_back(magicValue);
        if (checkWin(testMoves)) return move;
    }

    return availableMoves[rand() % availableMoves.size()];
}

int main() {
    srand(time(0));
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    vector<int> playerMoves, computerMoves;
    vector<pair<int, int>> availableMoves; 

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            availableMoves.push_back({i, j});
        }
    }

    cout << "Welcome to Tic Tac Toe (Magic Board Method)!\n";
    displayBoard(board);

    while (!availableMoves.empty()) {
        int row, col;
        cout << "Your move (row and column: 0-2): ";
        cin >> row >> col;

        auto it = find(availableMoves.begin(), availableMoves.end(), make_pair(row, col));
        if (it == availableMoves.end() || row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            cout << "Invalid move! Try again.\n";
            continue;
        }

        board[row][col] = 'X';
        int magicValue = magicSquare[row][col];
        playerMoves.push_back(magicValue);
        availableMoves.erase(it);

        displayBoard(board);

        if (checkWin(playerMoves)) {
            cout << "You win! Congratulations!\n";
            break;
        }


        //computer
        if (!availableMoves.empty()) {
            pair<int, int> computerMove = findBestMove(playerMoves, computerMoves, availableMoves);
            int compRow = computerMove.first;
            int compCol = computerMove.second;

            board[compRow][compCol] = 'O';
            int compMagicValue = magicSquare[compRow][compCol];
            computerMoves.push_back(compMagicValue);
            availableMoves.erase(find(availableMoves.begin(), availableMoves.end(), computerMove));

            cout << "Computer's move:\n";
            displayBoard(board);

            if (checkWin(computerMoves)) {
                cout << "Computer wins! Better luck next time.\n";
                break;
            }
        }
    }

    if (availableMoves.empty() && !checkWin(playerMoves) && !checkWin(computerMoves)) {
        cout << "It's a draw!\n";
    }

    return 0;
}
