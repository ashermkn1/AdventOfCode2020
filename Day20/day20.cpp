#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <optional>
#include <typeinfo>
#include <numeric>
using namespace std;
const int DIM = 12;
class Tile {
public:
    size_t id{};
    vector<optional<Tile>> adj;
    vector<vector<short>> data;
    // go clockwise starting from top
    size_t neighbours{};

    Tile(size_t id, const vector<vector<short>>& data) : id(id), data(data) {}

    Tile(size_t id, const pair<int, int>& size): id(id), data(size.first, vector<short>(size.second)) {}

    [[nodiscard]]
    vector<vector<short>> edges() const {
        vector<vector<short>> result;
        // top edge
        result.push_back(data[0]);
        // right edge
        vector<short> buffer;
        for (const auto& line : data) {
            buffer.push_back(line[line.size() - 1]);
        }
        result.push_back(buffer);

        buffer.clear();
        // bottom edge
        result.push_back(data[data.size() - 1]);

        // left edge
        for (const auto& line : data) {
            buffer.push_back(line[0]);
        }
        result.push_back(buffer);
        return result;
    }
    [[nodiscard]]
    vector<vector<short>> flipped_edges() const {
        auto result = edges();
        for (auto& edge : result) {
            reverse(edge.begin(), edge.end());
        }
        return result;
    }
    [[nodiscard]]
    // rotate 90 degrees clockwise
    Tile rotate() const {
        vector<vector<short>> new_data(data[0].size(), vector<short>(data.size()));
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data[0].size(); j++) {
                new_data[j][data.size() - i - 1] = data[i][j];
            }
        }
        return {id, new_data};
    }
    Tile& rotate()  {
        vector<vector<short>> new_data(data[0].size(), vector<short>(data.size()));
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data[0].size(); j++) {
                new_data[j][data.size() - i - 1] = data[i][j];
            }
        }
        this->data = new_data;
        std::rotate(adj.rbegin(), adj.rbegin() + 1, adj.rend());
        return *this;
    }
    Tile& flip_vert() {
        reverse(data.begin(), data.end());
        swap(adj[0], adj[2]);
        return *this;
    }
    [[nodiscard]]
    Tile flip_vert() const {
        vector<vector<short>> new_data;
        reverse_copy(data.begin(), data.end(), back_inserter(new_data));
        return {id, new_data};
    }
    Tile& flip_horiz() {
        for_each(data.begin(), data.end(), [](auto& row){reverse(row.begin(), row.end());});
        swap(adj[1], adj[3]);
        return *this;
    }
    [[nodiscard]]
    Tile flip_horiz() const {
        vector<vector<short>> new_data;
        copy(data.begin(), data.end(), back_inserter(new_data));
        for_each(new_data.begin(), new_data.end(), [](auto& row){reverse(row.begin(), row.end());});
        return {id, new_data};
    }
    Tile& transform(size_t rotations, bool flip = false) {
        for (size_t i = 0; i < rotations; i++) {
            rotate();
        }
        if (flip) {
            flip_vert();
        }
        return *this;
    }

    [[nodiscard]]
    Tile transform(size_t rotations, bool flip = false) const {
        Tile result = *this;
        for (size_t i = 0; i < rotations; i++) {
            result.rotate();
        }
        if (flip) {
            result.flip_vert();
        }
        return result;
    }
    vector<short>& operator[](size_t i) {
        return data[i];
    }
    void adjacents(const vector<Tile>& puzzle, int index) {
        for (int i = 0; i < 4; i++) {

            bool found = false;
            auto edge = edges()[i];
            for (int j = 0; j < puzzle.size(); j++) {
                auto other = puzzle[j].edges();
                auto flipped = puzzle[j].flipped_edges();
                if (j == index) {
                    continue;
                }

                if (find(other.begin(), other.end(), edge) != other.end() ||
                    find(flipped.begin(), flipped.end(), edge) != flipped.end()) {

                    adj.emplace_back(puzzle[j]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                adj.emplace_back(nullopt);
            }
        }

    }
    bool operator==(const Tile& other) const {
        return other.data == data;
    }
    void trim() {
        data.erase(data.begin());
        data.erase(data.end());
        for_each(data.begin(), data.end(), [](vector<short>& line){
            line.erase(line.begin());
            line.erase(line.end());
        });
    }
};

uint64_t corners(vector<Tile>& puzzle) {
    uint64_t result = 1;

    vector<vector<vector<short>>> edges;
    transform(puzzle.cbegin(), puzzle.cend(), back_inserter(edges), [](const auto& tile){return tile.edges();});

    vector<vector<vector<short>>> flipped;
    transform(puzzle.cbegin(), puzzle.cend(), back_inserter(flipped),
              [](const auto& tile){return tile.flipped_edges();});

    for (int i = 0; i < puzzle.size(); i++) {

        int unique = 0;
        for (const auto& edge : edges[i]) {
            int same = 0;

            for (int j = 0; j < puzzle.size(); j++) {
                if (i == j) {
                    continue;
                }
                for (const auto& other : edges[j]) {
                    if (other == edge) {
                        same++;
                    }
                }
                for (const auto& other : flipped[j]) {
                    if (other == edge) {
                        same++;
                    }
                }
            }
            if (!same) {
                unique++;

            }
        }
        if (unique == 2) {
            result *= puzzle[i].id;
        }
        puzzle[i].neighbours = 4 - unique;
    }
    return result;
}


size_t roughness(vector<Tile>& puzzle) {
    // 1) find top left corner and orient it
    // 2) match all other tiles, going row by row
    // 3) trim borders of tiles
    // 4) merge into one big tile
    // 5) find monsters
    vector<vector<Tile>> layout;
    layout.reserve(DIM);
    for (auto& c : layout) {
        c.reserve(DIM);
    }
    vector<Tile> row;
    // find top left corner
    row.push_back(*find_if(puzzle.begin(), puzzle.end(), [](const auto& tile){return tile.neighbours == 2;}));
    // orient corner
    row[0].transform(3);

    for (int i = 1; i < DIM - 1; i++) {

        auto next = row[i-1].adj[1].value();
        cout << typeid(next).name() << endl;
        while (next.adj[0].has_value()) {
            next.rotate();
        }
        if (row[i-1].edges()[1] != next.edges()[3]) {
            next.flip_horiz();
        }
        row.push_back(next);
    }
    // top right corner
    row.push_back(row[DIM-2].adj[1].value());
    while (row[DIM-1].adj[0].has_value() || row[DIM-1].adj[1].has_value()) {
        row[DIM-1].rotate();
    }
    layout.push_back(row);

    for (int i = 1; i < DIM; i++) {
        row.clear();
        row.push_back(layout[i-1][0].adj[2].value());
        auto& ref = row[0];
        while (!ref.adj[0].has_value() || ref.adj[0].value() != layout[i-1][0]) {
            ref.rotate();
        }

        if (ref.edges()[0] != layout[i-1][0].edges()[2]) {
            ref.flip_horiz();
        }

        for (int j = 1; j < DIM; j++) {
            auto next = layout[i][j-1].adj[1].value();

            while (!next.adj[0].has_value() || next.adj[0].value() != layout[i-1][j]) {
                next.rotate();
            }
            if (next.edges()[3] != layout[i][j-1].edges()[1]) {
                next.flip_horiz();
            }
            row.push_back(next);
        }
        layout.push_back(row);
    }

    // trim borders
    for (auto& r : layout) {
        for (auto& cell : r) {
            cell.trim();
        }
    }

    // build final image
    int x_length = 0;
    for (const auto& tile : layout[0]) {
        x_length += tile.edges()[0].size();
    }
    int y_length = 0;
    for (const auto& row : layout) {
        y_length += row[0].edges()[3].size();
    }
    Tile final(0, {y_length, x_length});

    int temp = 0;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < layout[i][0].edges()[3].size(); j++) {
            for (int k = 0; k < DIM; k++) {
                copy(layout[i][k].data[j].begin(), layout[i][k].data[j].end(), back_inserter(final.data[temp]));
            }
            temp++;
        }
    }
    // find monsters
    int monsters = 0;
    for (int i = 0; i < final.data.size() - 2; i++) {
        for (int j = 0; i < final.data[i].size() - 17; i++) {
            if (final[i][j+19] &&
                final[i+1][j+1] &&
                final[i+1][j+6] &&
                final[i+1][j+7] &&
                final[i+1][j+12] &&
                final[i+1][j+13] &&
                final[i+1][j+18] &&
                final[i+1][j+19] &&
                final[i+1][j+20] &&
                final[i+2][j+2] &&
                final[i+2][j+5] &&
                final[i+2][j+8] &&
                final[i+2][j+11] &&
                final[i+2][j+14] &&
                final[i+2][j+17]) {

                monsters++;
            }
        }
    }
    int hashes = 0;
    for (const auto& row : final.data) {
        hashes += count(row.begin(), row.end(), 1);
    }
    return hashes - (monsters * 15);
}
int main() {
    ifstream reader("input.txt");

    vector<string> lines;
    string buffer;
    while (getline(reader, buffer)) {
        lines.push_back(buffer);
    }

    vector<vector<string>> sections;
    vector<string> current;
    for (const auto& line : lines) {
        if (line.empty()) {
            sections.push_back(current);
            current.clear();
        } else {
            current.push_back(line);
        }
    }
    if (!current.empty()) {
        sections.push_back(current);
    }
    vector<Tile> puzzle;
    for (const auto& section : sections) {
        auto header = section[0];
        Tile tile {stoul(header.substr(5, 4)),
                   {section.size() - 1, section[1].size()}};

        for (int i = 1; i < section.size(); i++) {
            for (int j = 0; j < section[i].size(); j++) {
                tile.data[i - 1][j] = (section[i][j] == '#');
            }
        }
        puzzle.push_back(tile);
    }
    cout << "starting adjacents" << endl;
    for (int i = 0; i < puzzle.size(); i++) {
        cout << i << endl;
        puzzle[i].adjacents(puzzle, i);
    }
    cout << "starting part 1" << endl;

    auto p1s = chrono::high_resolution_clock::now();
    cout << "Part 1: " << corners(puzzle) << endl;
    auto p1d = chrono::high_resolution_clock::now();
    cout << "Took " << chrono::duration_cast<chrono::milliseconds>(p1d - p1s).count() << " ms" << endl;
    cout << "starting part 2" << endl;
    auto p2s = chrono::high_resolution_clock::now();
    cout << "Part 2: " << roughness(puzzle) << endl;
    auto p2d = chrono::high_resolution_clock::now();
    cout << "Took " << chrono::duration_cast<chrono::milliseconds>(p2d - p2s).count() << " ms" << endl;
}