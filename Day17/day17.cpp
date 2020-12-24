#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

#include "../ndarray.h"
#include "../common.h"

using namespace std;

NDArray<short> readInput(int turns, size_t ndim = 3);
void calc_neighbours(const NDArray<short>& plan, NDArray<short>& target, const index_t& source_index = {}, const index_t& target_index = {});
int main() {

}

NDArray<short> readInput(int turns, size_t ndim /*= 3*/) {
    ifstream reader("input.txt");

    vector<string> lines;
    string buffer;
    while (getline(reader, buffer)) {
        lines.push_back(buffer);
    }

    vector<size_t> shape(ndim, (size_t) 1 + 2 * turns);
    shape[0] = lines.size() + 2 * turns;
    shape[1] = lines[0].size() + 2 * turns;
    index_t offsets(ndim, -turns);

    NDArray<short> result(shape, offsets);
    for (size_t i = 0; i < lines.size(); i++) {
        auto line = lines[i];
        for (size_t j = 0; j < line.size(); j++) {
            index_t index(ndim, 0);
            index[0] = i;
            index[1] = j;

            switch (line[j]) {
                case '.':
                    result.set(index, 0);
                    break;
                case '#':
                    result.set(index, 1);
                    break;
                default:
                    throw runtime_error("Invalid token.");
            }
        }
    }
    return result;
}

void calc_neighbours(const NDArray<short> &plan, NDArray<short> &target, const index_t &source_index,
                     const index_t &target_index) {

    if (target_index.size() == plan.ndim()) {

    }
}
