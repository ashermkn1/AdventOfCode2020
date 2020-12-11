#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

using machine_t = vector<pair<char, int>>;

int part1(machine_t& instructions);
int part2(machine_t& instructions);

int main() {
    ifstream reader("input");

    string line;

    machine_t instructions;
    while (getline(reader, line)) {
        // parse integer
        int argument = stoi(line.substr(line.find(' ') + 1));

        instructions.emplace_back(line[0], argument);
    }

    cout << part1(instructions) << endl;
    cout << part2(instructions) << endl;
}

pair<bool, int> run_program(const machine_t& instructions) {
    int accumulator = 0;
    int current = 0;
    vector<bool> visited(instructions.size(), false);

    while ((current < instructions.size()) && !visited[current]) {

        visited[current] = true;

        const auto& instruction = instructions[current];
        switch (instruction.first) {

            case 'n':
                current++;
                break;

            case 'j':
                current += instruction.second;
                break;

            case 'a':
                accumulator += instruction.second;
                current++;
                break;
        }
    }
    return {current == instructions.size(), accumulator};
}
int part1(machine_t& instructions) {
    return run_program(instructions).second;
}

int part2(machine_t& instructions) {
    pair<bool, int> result;

    for (int i = 0; i < instructions.size(); i++) {
        machine_t copy = instructions;

        if (copy[i].first == 'a')
            continue;

        copy[i].first = copy[i].first == 'j'? 'n' : 'j';

        result = run_program(copy);

        if (result.first)
            break;
    }

    return result.second;
}
