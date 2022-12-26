#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
using ll = long long;
using tile = pair<ll, ll>;
static const unordered_map<string, tile> offsets = {
        {"e", { 1, 0}}, {"se", { 1,  1}}, {"sw", { 0,  1}},
        {"w", {-1, 0}}, {"nw", {-1, -1}}, {"ne", { 0, -1}}
};
int main() {
    unordered_map<tile, bool, hash_pair> tiles;

    ifstream reader("input.txt");

    vector<string> lines;
    string buffer;

    while (getline(reader, buffer)) {
        lines.push_back(buffer);
    }

    for (const auto& line : lines) {
        ll x = 0, y = 0;
        for (int i = 0; i < line.size();) {
            if (auto it = offsets.find(line.substr(i, 1)); it != offsets.end()) {
                auto [dx, dy] = it->second;
                x += dx;
                y += dy;
                i++;
            } else if (auto it = offsets.find(line.substr(i, 2)); it != offsets.end()) {
                auto [dx, dy] = it->second;
                x += dx;
                y += dy;
                i += 2;
            }
        }
        tiles[{x, y}] = !tiles[{x, y}];
    }
    cout << "Part 1: " << count_if(tiles.begin(), tiles.end(), [](const auto& kv){return kv.second;}) << endl;

    for (int i = 0; i < 100; i++) {
        unordered_map<tile, bool, hash_pair> next_gen;

        for (const auto& [pos, black] : tiles) {
            if (black) {
                next_gen[pos] = true;
                for (const auto& offset : offsets) {
                    auto [dx, dy] = offset.second;
                    next_gen.emplace(make_pair(pos.first + dx, pos.second + dy), false);
                }
            }
        }
        for (const auto& [pos, black] : next_gen) {
            ll count = 0;
            for (const auto& offset : offsets) {
                auto [dx, dy] = offset.second;
                if (tiles[{pos.first + dx, pos.second + dy}]) {
                    count++;
                }
            }
            if (black) {
                next_gen[pos] = (count > 0 && count <= 2);
            } else {
                next_gen[pos] = count == 2;
            }
        }
        tiles = move(next_gen);
    }
    cout << "Part 2: " << count_if(tiles.begin(), tiles.end(), [](const auto& kv){return kv.second;});
}