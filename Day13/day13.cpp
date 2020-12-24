#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <algorithm>

using namespace std;
typedef unsigned long long ull;
const ull NO_SERVICE = numeric_limits<ull>::max();

vector<string> split(const string& original, char delimiter = ' ', int maxSplits = -1)
{
    vector<string> result;
    string buffer;
    istringstream source(original);
    int splits = 0;

    while (getline(source, buffer, delimiter) && splits != maxSplits)
    {
        result.push_back(buffer);
        splits++;
    }
    return result;
}

inline ull wait_time(ull time, ull bus) {
    if (bus == NO_SERVICE)
        return NO_SERVICE;

    return bus - (time % bus);
}
struct Pattern {
    ull offset{0};
    ull factor{1};
};
void update_pattern(Pattern& pattern, ull bus, ull order) {
    if (bus == NO_SERVICE)
        return;

    ull modulo = (bus > order) ? (bus - order) % bus : bus - order % bus;

    while (pattern.offset % bus != modulo) {
        pattern.offset += pattern.factor;
    }
    pattern.factor *= bus;
}
int main() {

    ifstream reader("input.txt");

    ull start_time;
    string line;
    getline(reader, line);
    start_time = stoull(line);
    getline(reader, line);

    auto str_times = split(line, ',');

    vector<ull> times;
    for (const auto& bus : str_times) {
        cout << bus << endl;
        if (bus == "x") {
            times.push_back(NO_SERVICE);
        } else {
            times.push_back(stoull(bus));
        }
    }
    cout << times[0] << endl;
    ull first_bus = *min_element(times.cbegin(), times.cend(), [start_time](ull b1, ull b2){
        return wait_time(start_time, b1) < wait_time(start_time, b2);
    });
    cout << "Part 1: " << first_bus * wait_time(start_time, first_bus) << endl;

    Pattern pattern{};

    for (size_t i = 0; i < times.size(); i++) {
        update_pattern(pattern, times[i], (ull) i);
    }
    cout << "Part 2: " << pattern.offset;
}