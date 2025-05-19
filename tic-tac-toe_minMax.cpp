#include <iostream>
#include <vector>
using namespace std;

char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

void printBoard() {
    cout << "\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "--|---|--\n";
    }
    cout << "\n";
}

bool isWinning(char player) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||        // row
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))          // col
            return true;
    }
    if((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
           (board[0][2] == player && board[1][1] == player && board[2][0] == player))
           return true;
    
    return false;
}

bool isBoardFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') return false; 
    return true; 
}

int minimax(bool isMax) {
    if (isWinning('O')) return 1;   // AI wins
    if (isWinning('X')) return -1;  // Player wins
    if (isBoardFull()) return 0;    // Draw

    int bestScore = isMax ? -10 : 10;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = isMax ? 'O' : 'X';
                int score = minimax(!isMax);
                board[i][j] = ' '; 
                bestScore = isMax ? max(bestScore, score) : min(bestScore, score);
            }
        }
    }
    return bestScore;
}

void makeBestMove() {
    int bestScore = -10, moveRow = -1, moveCol = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int score = minimax(false);
                board[i][j] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    moveRow = i;
                    moveCol = j;
                }
            }
        }
    }
    if (moveRow != -1 && moveCol != -1)
        board[moveRow][moveCol] = 'O';
}

int main() {
    int row, col;
    bool gameRunning = true;
    while (gameRunning) {
        printBoard();

        // Player
        cout << "Enter your move (row and column 0-2): ";
        cin >> row >> col;
        if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            cout << "Invalid move! Try again.\n";
            continue;
        }
        board[row][col] = 'X';

        if (isWinning('X')) {
            printBoard();
            cout << "You win!\n";
            gameRunning = false;
            continue;
        }
        if (isBoardFull()) {
            printBoard();
            cout << "It's a draw!\n";
            gameRunning = false;
            continue;
        }

        // AI
        makeBestMove();

        if (isWinning('O')) {
            printBoard();
            cout << "AI wins!\n";
            gameRunning = false;
            continue;
        }
        if (isBoardFull()) {
            printBoard();
            cout << "It's a draw!\n";
            gameRunning = false;
            continue;
        }
    }
    return 0;
}











//isMax = true means it's AI's turn , try to maximise = max(bs, score)
//Player = isMax = false, try to minimize = min(bs, score)