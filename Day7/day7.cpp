#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <regex>
#include <queue>
using namespace std;

using contained_t = unordered_map<string, vector<string>>;
using contains_t = unordered_map<string, unordered_map<string, int>>;

int part1(contained_t& adj, const string& color);

unsigned int count_bags(contains_t &adj, const string& color);

int main() {
    // part 1
    contained_t contained;
    // part 2
    contains_t contains;

    ifstream reader("input");

    string buffer;
    smatch root_match;
    // root_match first two words
    regex first2words("^\\w+ \\w+");
    // root_match all bags that can be contained
    regex edges(R"((\d) (\w+ \w+))");

    // read each line
    while (getline(reader, buffer)) {

        regex_search(buffer, root_match, first2words);

        contains.emplace(root_match[0], unordered_map<string, int>());
        // get all matches in string, not just first
        sregex_iterator start(buffer.begin(), buffer.end(), edges);
        sregex_iterator end;

        for (sregex_iterator i = start; i != end; ++i) {

            smatch match = *i;

            contains[root_match[0]].emplace(match[2], stoi(match[1]));

            contained[match[2]].push_back(root_match[0]);
        }
    }
    cout << "Part 1: " << part1(contained, "shiny gold") << endl;
    cout << "Part 2: " << count_bags(contains, "shiny gold") << endl;
}

unsigned int count_bags(contains_t& adj, const string& color) {
    unsigned int bag_count = 0;

    for (auto& [bag_name, count] : adj[color]) {
        bag_count += count + count * count_bags(adj, bag_name);
    }
    return bag_count;
}

int part1(contained_t& adj, const string& color) {

    unordered_set<string> explored;
    queue<string> frontier;

    frontier.push(color);
    // while nodes to explore
    while (!frontier.empty()) {

        string current = frontier.front();
        frontier.pop();
        // all neighbours of current node
        for (const string& neighbour: adj[current]) {
            // make sure we haven't explored it yet
            if (explored.find(neighbour) == explored.end()) {
                frontier.push(neighbour);
                explored.insert(neighbour);
            }
        }
    }
    return explored.size();
}