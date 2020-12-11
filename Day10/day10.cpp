#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <chrono>
#include <unordered_map>
typedef long long ll;
using namespace std;

int diffs(const vector<int>& joltages);

ll ways(int from, const vector<int>& joltages);

int main() {
    const auto read = chrono::high_resolution_clock::now();
    ifstream reader("input.txt");

    int buffer;

    vector<int> joltages;

    while (reader >> buffer)
        joltages.push_back(buffer);

    const auto end_read = chrono::high_resolution_clock::now();

    cout << "Reading the file took "
         << chrono::duration_cast<chrono::nanoseconds>(end_read - read).count()
         << " ns" << endl;


    const auto part1 = chrono::high_resolution_clock::now();
    // sort adapters
    sort(joltages.begin(), joltages.end());
    // source has joltage of 0
    joltages.insert(joltages.begin(), 0);
    // device has joltage of highest adapter + 3
    joltages.push_back(joltages[joltages.size() - 1] + 3);

    cout << "Part 1: " << diffs(joltages) << endl;

    const auto end_part1 = chrono::high_resolution_clock::now();

    cout << "Part 1 "
         << " took "  << chrono::duration_cast<chrono::nanoseconds>(end_part1 - part1).count()
         << " ns" << endl;

    const auto part2 = chrono::high_resolution_clock::now();
    cout << "Part 2: " << ways(0, joltages) << endl;

    const auto end_part2 = chrono::high_resolution_clock::now();

    cout << "Part 2 "
         << " took "  << chrono::duration_cast<chrono::nanoseconds>(end_part2 - part2).count()
         << " ns" << endl;
}

ll ways(int from, const vector<int>& joltages) {

    static unordered_map<int, ll> cache;

    if (cache.find(from) != cache.end()) {
        return cache.at(from);
    }
    // if adapter doesnt exist
    if (!binary_search(joltages.begin(), joltages.end(), from)) {
        return 0;
    }
    // device
    if (from == joltages[joltages.size()-1]) {
        return 1;
    }

    ll sum = 0;
    // loop through all valid adapters
    for (int i = from + 1; i <= from + 3; i++) {
        sum += ways(i, joltages);
    }
    cache[from] = sum;
    return sum;
}

int diffs(const vector<int>& joltages) {

    vector<int> diffs;

    adjacent_difference(joltages.cbegin(), joltages.cend(), back_inserter(diffs));

    int one_diffs = count_if(diffs.cbegin(), diffs.cend(), [](const int i){return i == 1;});
    int three_diffs = count_if(diffs.cbegin(), diffs.cend(), [](const int i){return i == 3;});

    return one_diffs * three_diffs;
}
