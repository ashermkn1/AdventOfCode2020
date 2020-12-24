#include <iostream>
#include <fstream>
#include <unordered_map>
#include <regex>
#include <algorithm>
using namespace std;
using val_t = bitset<36>;
val_t apply_mask(val_t value, const string& mask);
vector<val_t> decode(val_t address, const string& mask);
int main() {
    // grabs memory address and value
    regex assignment(R"(mem\[(\d+)\] = (\d+))");
    // grabs bitmask
    regex mask_r("mask = ([\\dX]+)");
    smatch results;

    unordered_map<val_t , val_t> mem;
    unordered_map<val_t, val_t> memv2;
    ifstream reader("input.txt");

    string line;
    string mask;

    while (getline(reader, line)) {
        // line sets the mask
        if (line[1] == 'a') {
            regex_search(line, results, mask_r);

            mask = results[1];
        } else {
            regex_search(line, results, assignment);

            val_t address(stoull(results[1].str()));
            val_t value(stoull(results[2].str()));

            mem[address] = apply_mask(value, mask);


            for (const auto& ad : decode(address, mask)) {
                memv2[ad] = value;
            }
        }
    }
    uint64_t sum = 0;
    for (const auto& element : mem) {
        sum += element.second.to_ullong();
    }
    uint64_t sum2 = 0;
    for (const auto& element : memv2) {
        sum2 += element.second.to_ullong();
    }
    cout << "Part 1: " << sum << endl;
    cout << "Part 2: " << sum2 << endl;
}
val_t apply_mask(val_t value, const string& mask) {
    string on(mask);
    string off(mask);

    replace(on.begin(), on.end(), 'X', '0');
    replace(off.begin(), off.end(), 'X', '1');

    return (value | val_t(on)) & val_t(off);
}

vector<val_t> decode(val_t address, const string& mask) {
    vector<val_t> adresses;

    string on(mask);
    replace(on.begin(), on.end(), 'X', '0');

    address |= val_t(on);
    // bit to float, by what to left shift the 0 or 1
    vector<size_t> floats;

    for (size_t i = 0; i < mask.length(); i++) {
        if (mask[i] == 'X')
            floats.push_back(mask.length() - i - 1);
    }
    // so we can index same way as bitset
    reverse(floats.begin(), floats.end());

    for (unsigned long long i = 0; i < pow(2, floats.size()); i++) {
        val_t current(address);
        for (size_t j = 0; j < floats.size(); j++) {
            current[floats[j]] = (i >> j) & 1;
        }
        adresses.push_back(current);
    }
    return adresses;
}