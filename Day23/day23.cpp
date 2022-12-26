#include <iostream>
#include <vector>
#
using namespace std;

void crab_cups(vector<int>& cups, const int rounds = 10000000) {
    auto current = cups.begin();
    int min = 1;
    int max = 1000000;
    for (int _ = 0; _ < rounds; _++) {

        vector<int> temp;

        // remove next 3 cups
        for (int i = 0; i < 3; i++) {
            auto temp_it = current + 1;
            if (temp_it == cups.end()) {
                temp_it = cups.begin();
            }
            temp.push_back(*temp_it);
            cups.erase(temp_it);
        }

        vector<int>::iterator elem;
        int label = *current;
        do {
            // subtract one and wrap in range
            label--;
            if (label < min) {
                label = max;
            }
            elem = find(cups.begin(), cups.end(), label);

        } while (elem == cups.end());
        cups.insert(elem + 1, temp.begin(), temp.end());

        current++;
        if (current == cups.end()) {
            current = cups.begin();
        }
    }
}
int main() {
    vector<int> cups = {2,8,4,5,7,3,9,6,1};
    for (int i = 10; i <= 1000000; i++) {
        cups.push_back(i);
    }
    crab_cups(cups);
    uint64_t result = 1;
    auto it = find(cups.begin(), cups.end(), 1);

    it++;
    if (it == cups.end()) {
        it = cups.begin();
    }
    result *= *it;
    it++;
    if (it == cups.end()) {
        it = cups.begin();
    }
    result *= *it;

    cout << result << endl;
}