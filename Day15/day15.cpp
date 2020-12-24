#include <algorithm>
#include <vector>
#include <iostream>
#include <chrono>
#include <map>
using namespace std;
unsigned int play_game(const vector<int>& input, unsigned int max_turn);
unsigned long long play_game_long(const vector<int>& input, unsigned int max_turn);
using timer = chrono::high_resolution_clock;
int main() {

    const vector<int> input = {16,1,0,18,12,14,19};

    const auto& start1 = timer::now();
    cout << "Part 1: " << play_game(input, 2020) << endl;
    const auto& end1 = timer::now();
    cout << "Part 1 took "
         << chrono::duration_cast<chrono::milliseconds>(end1 - start1).count()
         << " ms" << endl;

    const auto& start2 = timer::now();
    cout << "Part 2: " << play_game_long(input, 30000000) << endl;
    const auto& end2 = timer::now();
    cout << "Part 2 took "
         << chrono::duration_cast<chrono::milliseconds>(end2 - start2).count()
         << " ms" << endl;
}

unsigned long long play_game_long(const vector<int>& input, unsigned int max_turn) {
    map<int, int> history;

    for (int i = 0; i < input.size() - 1; i++) {
        history[input[i]] = i;
    }
    int prev_num = *input.rbegin();
    for (int turn = input.size(); turn < max_turn; turn++) {
        int next_num = 0;

        if (history.contains(prev_num)) {
            next_num = turn - history.at(prev_num) - 1;
        }
        history[prev_num] = turn - 1;
        prev_num = next_num;
    }
    return prev_num;
}
unsigned int play_game(const vector<int>& input, unsigned int max_turn) {
    vector<int> history = input;

    for (int i = input.size(); i < 2020; i++) {
        int prev_num = *history.rbegin();

        auto it = find(history.rbegin() + 1, history.rend(), prev_num);

        if (it == history.rend()) {
            history.push_back(0);
        } else {
            history.push_back(it - history.rbegin());
        }
    }
    return *history.rbegin();
}