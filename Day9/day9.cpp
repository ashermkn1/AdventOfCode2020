#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

typedef unsigned long long ull;
bool xmas_check(ull to_check, const vector<ull>& nums);

ull contiguous_sum(ull target, vector<ull>& nums);

const int PREAMBLE = 25;
int main() {
    const auto start = chrono::high_resolution_clock::now();
    ifstream reader("input");

    bool first = true;
    ull target;
    vector<ull> nums;

    nums.reserve(1000);

    ull buff;
    while (reader >> buff)
        nums.emplace_back(buff);

    const auto end = chrono::high_resolution_clock::now();
    cout << "Reading file took " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() / (double) 1E9 << " seconds" << endl;
    int starting_num = 0;
    for (size_t i = PREAMBLE; i < nums.size(); i++) {
        // capture previous 25 numbers
        vector<ull> earlier(nums.begin() + starting_num, nums.begin() + i);

        if (!xmas_check(nums[i], earlier)) {
            if (first) {
                cout << "Part 1: " << nums[i] << endl;
                target = nums[i];
                first = false;
            }
        }
        starting_num++;
    }
    // erase all numbers bigger than target
    erase_if(nums, [target](ull i) {return i > target;});


    cout << "Part 2: " << contiguous_sum(target, nums) << endl;
    const auto prog_end = chrono::high_resolution_clock::now();
    cout << "Took " << chrono::duration_cast<chrono::nanoseconds>(prog_end - start).count() / (double) 1E9 << " seconds" << endl;
}

ull contiguous_sum(ull target, vector<ull>& nums) {
    int size = nums.size();

    pair<int, int> sum_bounds;

    ull curr_sum = nums[0];
    int start = 0;

    for (int i = 1; i <= size; i++) {
        // if sum is bigger than target, advance starting point of subvector
        while (curr_sum > target && start < i - 1) {
            curr_sum = curr_sum - nums[start];
            start++;
        }

        if (curr_sum == target) {
            sum_bounds = {start, i - 1};
            break;
        }

        if (i < size)
            curr_sum += nums[i];
    }

    vector<ull> sum(nums.begin() + sum_bounds.first, nums.begin() + sum_bounds.second);

    auto [min, max] = minmax_element(sum.begin(), sum.end());
    return *min + *max;
}

bool xmas_check(const ull to_check, const vector<ull>& nums) {

    return any_of(nums.begin(), nums.end(), [to_check, nums](ull i) {
        return find(nums.begin(), nums.end(), to_check - i) != nums.end() && i*2 != to_check;
    });
}