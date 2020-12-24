#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "../common.h"
using namespace std;

struct Range {
    int low;
    int high;

    [[nodiscard]] bool matches(int num) const {
        return num >= low && num <= high;
    }
};
struct DoubleRange {
    Range first;
    Range second;

    [[nodiscard]] bool matches(int num) const {
        return first.matches(num) || second.matches(num);
    }
};
Range parse_range(const string& s) {
    auto parts = split(s, '-');
    return {stoi(parts[0]), stoi(parts[1])};
}
vector<int> parse_ticket(const string& line) {
    auto parts = split(line, ',');
    vector<int> ticket;
    transform(parts.cbegin(), parts.cend(), back_inserter(ticket), [](const auto& part){return stoi(part);});
    return ticket;
}
bool valid(const vector<int>& ticket, const unordered_map<string, DoubleRange>& rules) {
    for (auto number : ticket) {
        if (!any_of(rules.cbegin(), rules.cend(),[number](const auto& rule){
            return rule.second.matches(number);
        })) {
            return false;
        }
    }
    return true;
}
vector<vector<int>> valid_tickets(const vector<vector<int>>& nearby, const unordered_map<string, DoubleRange>& rules) {
    vector<vector<int>> result;

    copy_if(nearby.cbegin(), nearby.cend(), back_inserter(result),[nearby, rules](const auto& ticket) {
        return valid(ticket, rules);
    });
    return result;
}
int main() {
    ifstream reader("input.txt");

    unordered_map<string, DoubleRange> rules;
    vector<int> my_ticket;
    vector<vector<int>> nearby_tickets;

    string line;

    vector<string> lines;
    vector<vector<string>> sections;

    while (getline(reader, line)) {
        if (line.empty()) {
            sections.push_back(lines);
            lines = {};
        } else {
            lines.push_back(line);
        }
    }
    sections.push_back(lines);
    // rules
    for (const auto& rule: sections[0]) {
        auto name_range = split(rule, ": ");

        auto ranges = split(name_range[1], " or ");
        rules[name_range[0]] = {parse_range(ranges[0]), parse_range(ranges[1])};
    }

    my_ticket = parse_ticket(sections[1][1]);
    transform(
            sections[2].cbegin() + 1,
            sections[2].cend(),
            back_inserter(nearby_tickets),
            parse_ticket
            );

    int result = 0;

    for (const auto& ticket : nearby_tickets) {
        for (const int number : ticket) {
            if (!any_of(rules.cbegin(),rules.cend(),[number](const auto& rule) {
                        return rule.second.matches(number);})) {
                result += number;
            }

        }
    }
    cout << "Part 1: " << result << endl;

    auto valids = valid_tickets(nearby_tickets, rules);
    valids.push_back(my_ticket);
    unordered_map<int, vector<string>> matches;

    for (const auto& element : rules) {
        const auto& name = element.first;
        const auto& rule = element.second;

        for (int i = 0; i < my_ticket.size(); i++) {
            bool match = true;
            for (const auto& ticket : valids) {
                if (!rule.matches(ticket[i])) {
                    match = false;
                    break;
                }
            }
            if (match)
                matches[i].push_back(name);
        }
    }
    unordered_map<string, int> order;

    while (order.size() != my_ticket.size()) {
        for (const auto& kv : matches) {
            if (kv.second.size() == 1) {
                string name = kv.second[0];
                order[name] = kv.first;

                for (auto& kv2 : matches) {
                    const auto& matchings = kv2.second;
                    vector<string> new_matchings;

                    remove_copy_if(matchings.cbegin(), matchings.cend(), back_inserter(new_matchings),[name](auto s){return name == s;});
                    matches[kv2.first] = new_matchings;
                }
                break;
            }
        }
    }

    long long mul = 1;

    for (const auto& kv : order) {
        if (kv.first.substr(0, 9) == "departure") {
            mul *= my_ticket[kv.second];
        }
    }

    cout << "part 2: " << mul << endl;
}

