#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;
const int door = 335121;
const int card = 363891;
uint64_t loop_size(const int public_key, const int subject = 7) {
    uint64_t val = 1;
    uint64_t i;
    for (i = 0; val != public_key; i++) {
        val *= subject;
        val %= 20201227;
    }
    return i;
}
uint64_t transform(const int subject, uint64_t size) {
    uint64_t val = 1;
    for (int i = 0; i < size; i++) {
        val *= subject;
        val %= 20201227;
    }
    return val;
}
int main() {
    cout << transform(door, loop_size(card)) << endl;
}