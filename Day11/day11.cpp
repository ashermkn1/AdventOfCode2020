#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

enum class SeatState: char {
    empty = 'L',
    occupied = '#',
    unavailable = '.'
};
using Row = vector<SeatState>;
using Matrix = vector<Row>;

size_t count_occupied(const Matrix& seats);
size_t count_adjacent(const Matrix& seats, size_t row, size_t column);
size_t count_direction(const Matrix& seats, size_t row, size_t column);
// one cycle of simulation
void update_seats(const Matrix& prev_seats, Matrix& next, decltype(count_adjacent) counter = count_adjacent, size_t occupied_limit = 4);

ostream& operator<<(ostream& out, const Matrix& seats);

void parse_lines(const vector<string>& lines, Matrix& seats);

void stabilize(Matrix& seats, bool part2 = false);

int main() {
    const auto read_start = chrono::high_resolution_clock::now();
    ifstream reader("input.txt");

    vector<string> lines;

    string buffer;

    while (reader >> buffer)
        lines.emplace_back(buffer);

    const auto read_stop = chrono::high_resolution_clock::now();
    cout << "Reading the file took "
         << chrono::duration_cast<chrono::nanoseconds>(read_stop - read_start).count()
         << " ns" << endl;

    Matrix seats;
    parse_lines(lines, seats);
    Matrix original = seats;

    const auto stab_start = chrono::high_resolution_clock::now();

    stabilize(seats);

    const auto stab_stop = chrono::high_resolution_clock::now();
    cout << "Part 1 took "
         << chrono::duration_cast<chrono::milliseconds>(stab_stop - stab_start).count()
         << " ms" << endl;

    const auto stab2_start = chrono::high_resolution_clock::now();

    stabilize(original, true);

    const auto stab2_stop = chrono::high_resolution_clock::now();

    cout << "Part 2 took "
         << chrono::duration_cast<chrono::milliseconds>(stab2_stop - stab2_start).count()
         << " ms" << endl;
}

void stabilize(Matrix& seats, bool part2 /*= false*/) {
    while (true) {
        Matrix next;
        if (part2) {
            update_seats(seats, next, count_direction, 5);
        } else {
            update_seats(seats, next);
        }

        if (next == seats)
            break;

        seats = next;
    }
    cout << "Part " << (part2? "2" : "1") << ": " << count_occupied(seats) << endl;
}

void parse_lines(const vector<string>& lines, Matrix& seats) {
    seats.clear();
    for (const auto& line: lines) {
        Row row;
        for (const char c : line) {
            row.push_back(SeatState(c));
        }
        seats.push_back(row);
    }

}

void update_seats(const Matrix& prev_seats, Matrix& next, decltype(count_adjacent) counter/* = count_adjacent*/, size_t occupied_limit /*= 4*/) {
    next.clear();

    for (size_t i = 0; i < prev_seats.size(); i++) {
        const auto& prev_row = prev_seats[i];
        Row new_row;

        for (size_t j = 0; j < prev_row.size(); j++) {
            SeatState prev_state = prev_row[j];
            SeatState new_state = prev_state;
            // get count of occupied adjacent seats
            auto occupied = counter(prev_seats, i, j);

            switch (prev_state) {
                case SeatState::empty:
                    if (occupied == 0)
                        new_state = SeatState::occupied;
                    break;

                case SeatState::occupied:
                    if (occupied >= occupied_limit)
                        new_state = SeatState::empty;
                    break;
            }
            new_row.push_back(new_state);
        }
        next.push_back(new_row);
    }
}

size_t count_occupied(const Matrix &seats) {
    size_t count = 0;
    for (const auto& row: seats) {
        for (const auto& seat: row) {
            if (seat == SeatState::occupied)
                count++;
        }
    }
    return count;
}

size_t count_adjacent(const Matrix& seats, size_t row, size_t column) {
    size_t count = 0;
    // inline the out of bounds checks
    for (int i = max(0, (int) row - 1); i <= min(seats.size() - 1, row + 1); i++) {
        for (int j = max(0, (int) column - 1); j <= min(seats[i].size() - 1, column + 1); j++) {
            // only check 8 surrounding
            if (i == row && j == column)
                continue;

            if (seats[i][j] == SeatState::occupied)
                count++;
        }
    }
    return count;
}

size_t count_direction(const Matrix& seats, size_t row, size_t column) {
    size_t count = 0;

    const static vector<pair<int, int>> directions = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0, -1},
            {0, 1},
            {1, -1},
            {1, 0},
            {1, 1}
    };

    for (const auto& direction : directions) {
        int i = row;
        int j = column;

        i += direction.first;
        j += direction.second;

        while (i >= 0 && j >= 0 && i < seats.size() && j < seats[i].size()) {
            auto seat = seats[i][j];

            if (seat == SeatState::unavailable) {
                i += direction.first;
                j += direction.second;

                continue;
            }

            if (seat == SeatState::occupied) {
                count++;
            }
            break;
        }
    }

    return count;
}
ostream& operator<<(ostream& out, const Matrix& seats) {
    for (const auto &row : seats) {
        for (auto seat : row) {
            out << (char)seat;
        }
        out << endl;
    }
    return out;
}
