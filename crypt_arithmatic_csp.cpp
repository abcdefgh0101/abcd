#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

class CryptarithmeticSolver {
private:
    string word1, word2, result;
    vector<char> letters;
    unordered_map<char, unordered_set<int>> domains;
    unordered_map<char, int> assignment;
    int solutions;

    void collectUniqueLetters() {
        unordered_set<char> letterSet;
        for (char c : word1) letterSet.insert(c);
        for (char c : word2) letterSet.insert(c);
        for (char c : result) letterSet.insert(c);
        letters.assign(letterSet.begin(), letterSet.end());
    }

    void initializeDomains() {
        for (char c : letters) {
            domains[c] = unordered_set<int>();
            for (int d = 0; d <= 9; d++) {
                domains[c].insert(d);
            }
        }

        // Apply initial constraints
        if (!word1.empty()) domains[word1[0]].erase(0);
        if (!word2.empty()) domains[word2[0]].erase(0);
        if (!result.empty()) domains[result[0]].erase(0);

        if (result.size() > max(word1.size(), word2.size())) {
            if (!result.empty()) {
                domains[result[0]].clear();
                domains[result[0]].insert(1);
            }
        }
    }

    bool isConsistent() {
        if (assignment.count(word1[0]) && assignment[word1[0]] == 0) return false;
        if (assignment.count(word2[0]) && assignment[word2[0]] == 0) return false;
        if (assignment.count(result[0]) && assignment[result[0]] == 0) return false;

        unordered_set<int> usedValues;
        for (auto& pair : assignment) {
            if (usedValues.count(pair.second)) return false;
            usedValues.insert(pair.second);
        }

        return true;
    }

    char selectUnassignedVariable() {
        char minVar = '\0';
        int minSize = 10;
        
        for (char c : letters) {
            if (!assignment.count(c) && domains.count(c) && !domains[c].empty() && domains[c].size() < minSize) {
                minSize = domains[c].size();
                minVar = c;
            }
        }
        
        return minVar;
    }

    bool forwardCheck(char var, int value) {
        unordered_map<char, unordered_set<int>> oldDomains = domains;
        
        for (char c : letters) {
            if (c != var && !assignment.count(c) && domains.count(c)) {
                domains[c].erase(value);
                if (domains[c].empty()) {
                    domains = oldDomains;
                    return false;
                }
            }
        }

        if (!applyColumnConstraints()) {
            domains = oldDomains;
            return false;
        }

        return true;
    }

    bool applyColumnConstraints() {
        for (int col = 0; col < result.size(); col++) {
            vector<char> vars;
            int w1_pos = word1.size() - 1 - col;
            int w2_pos = word2.size() - 1 - col;
            int res_pos = result.size() - 1 - col;

            if (w1_pos >= 0) vars.push_back(word1[w1_pos]);
            if (w2_pos >= 0) vars.push_back(word2[w2_pos]);
            vars.push_back(result[res_pos]);

            int assignedCount = 0;
            int sum = 0;
            int carry = 0;

            for (char c : vars) {
                if (assignment.count(c)) {
                    assignedCount++;
                    if (c == result[res_pos]) {
                        sum -= assignment[c];
                    } else {
                        sum += assignment[c];
                    }
                }
            }

            if (assignedCount == vars.size() - 1) {
                char unassigned = '\0';
                for (char c : vars) {
                    if (!assignment.count(c)) {
                        unassigned = c;
                        break;
                    }
                }

                if (unassigned == '\0') continue;

                int requiredValue;
                if (unassigned == result[res_pos]) {
                    requiredValue = sum + carry;
                } else {
                    requiredValue = assignment[result[res_pos]] - (sum + carry);
                }

                requiredValue = (requiredValue + 10) % 10;

                if (domains.count(unassigned) && domains[unassigned].count(requiredValue)) {
                    domains[unassigned].clear();
                    domains[unassigned].insert(requiredValue);
                } else {
                    return false;
                }
            }
        }
        return true;
    }

    bool cspSolver() {
        if (assignment.size() == letters.size()) {
            if (isSolution()) {
                solutions++;
                printSolution();
                return true;
            }
            return false;
        }

        char var = selectUnassignedVariable();
        if (var == '\0') {
            return false;
        }

        if (domains.find(var) == domains.end() || domains[var].empty()) {
            return false;
        }

        unordered_set<int> currentDomain = domains[var];
        
        for (int value : currentDomain) {
            if (usedInAssignment(value)) {
                continue;
            }

            assignment[var] = value;
            unordered_map<char, unordered_set<int>> oldDomains = domains;

            if (forwardCheck(var, value)) {
                if (cspSolver()) {
                    return true;
                }
            }

            assignment.erase(var);
            domains = oldDomains;
        }

        return false;
    }

    bool usedInAssignment(int value) {
        for (auto& pair : assignment) {
            if (pair.second == value) return true;
        }
        return false;
    }

    bool isSolution() {
        long num1 = 0, num2 = 0, res = 0;

        for (char c : word1) {
            if (!assignment.count(c)) return false;
            num1 = num1 * 10 + assignment[c];
        }

        for (char c : word2) {
            if (!assignment.count(c)) return false;
            num2 = num2 * 10 + assignment[c];
        }

        for (char c : result) {
            if (!assignment.count(c)) return false;
            res = res * 10 + assignment[c];
        }

        return num1 + num2 == res;
    }

    void printSolution() {
        cout << "\nFound solution:" << endl;
        
        for (char letter : letters) {
            cout << letter << " = " << assignment[letter] << endl;
        }
        
        cout << "\nEquation verification:" << endl;
        cout << word1 << ": ";
        for (char c : word1) cout << assignment[c];
        cout << endl;
        cout << word2 << ": ";
        for (char c : word2) cout << assignment[c];
        cout << endl;
        cout << result << ": ";
        for (char c : result) cout << assignment[c];
        cout << endl;
    }

public:
    CryptarithmeticSolver(const string& w1, const string& w2, const string& res) 
        : word1(w1), word2(w2), result(res), solutions(0) {
        
        if (w1.empty() || w2.empty() || res.empty()) {
            cerr << "Error: Empty input words" << endl;
            exit(1);
        }
        
        collectUniqueLetters();
        initializeDomains();
        
        cout << "Solving: " << word1 << " + " << word2 << " = " << result << endl;
        cout << "Unique letters: ";
        for (char c : letters) cout << c << " ";
        cout << endl;
    }
    
    bool solve() {
        return cspSolver();
    }
    
    int getSolutionsCount() {
        return solutions;
    }
};

int main() {
    string word1, word2, result;
    
    cout << "Enter the first word (e.g., SEND): ";
    cin >> word1;
    
    cout << "Enter the second word (e.g., MORE): ";
    cin >> word2;
    
    cout << "Enter the result word (e.g., MONEY): ";
    cin >> result;
    
    transform(word1.begin(), word1.end(), word1.begin(), ::toupper);
    transform(word2.begin(), word2.end(), word2.begin(), ::toupper);
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    
    CryptarithmeticSolver solver(word1, word2, result);
    
    if (solver.solve()) {
        cout << "\nPuzzle successfully solved!" << endl;
    } else {
        cout << "\nNo solution exists for this puzzle." << endl;
    }
    
    return 0;
}