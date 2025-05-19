#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

struct State {
    int missionaries, cannibals, boat;
    State* parent;
};

State* createState(int m, int c, int b, State* p) {
    State* newState = new State{m, c, b, p};
    return newState;
}

bool isValid(State* state) {
    if (state->missionaries < 0 || state->cannibals < 0 || state->missionaries > 3 || state->cannibals > 3)
        return false;
    if ((state->missionaries < state->cannibals && state->missionaries > 0) ||
        (3 - state->missionaries < 3 - state->cannibals && (3 - state->missionaries) > 0))
        return false;
    return true;
}

bool isGoal(State* state) {
    return state->missionaries == 0 && state->cannibals == 0 && state->boat == 0;
}

vector<State*> getNextStates(State* current) {
    vector<State*> nextStates;
    int moves[5][2] = {{1, 0}, {2, 0}, {0, 1}, {0, 2}, {1, 1}};
    
    for (auto &move : moves) {
        int newM = current->missionaries + (current->boat == 1 ? -move[0] : move[0]);
        int newC = current->cannibals + (current->boat == 1 ? -move[1] : move[1]);
        int newB = 1 - current->boat;
        State* newState = createState(newM, newC, newB, current);
        if (isValid(newState)) {
            nextStates.push_back(newState);
        } else {
            delete newState;
        }
    }
    return nextStates;
}

void printSolution(State* state) {
    vector<State*> path;
    while (state) {
        path.push_back(state);
        state = state->parent;
    }
    reverse(path.begin(), path.end());
    for (auto s : path) {
        cout << "(" << s->missionaries << "," << s->cannibals << "," << s->boat << ")" << endl;
    }
}

void bfs() {
    queue<State*> queue;
    set<string> visited;
    State* initialState = createState(3, 3, 1, nullptr);
    queue.push(initialState);
    visited.insert("3,3,1");
    
    while (!queue.empty()) {
        State* current = queue.front();
        queue.pop();
        
        if (isGoal(current)) {
            printSolution(current);
            return;
        }

        for (State* next : getNextStates(current)) {
            string stateString = to_string(next->missionaries) + "," + to_string(next->cannibals) + "," + to_string(next->boat);
            if (visited.find(stateString) == visited.end()) {
                queue.push(next);
                visited.insert(stateString);
            } else {
                delete next;
            }
        }
    }
}

int main() {
    bfs();
    return 0;
}