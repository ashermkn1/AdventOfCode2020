#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <regex>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../common.h"

const int recursionLevel = 32;
using namespace std;
class Rule {
public:
    virtual string pattern() const = 0;

    bool matches(const std::string &s) const {
        std::regex re(pattern());
        return regex_match(s, re);
    }
};

class Literal : public Rule {
public:
    Literal(char c) : _character(c) {}

    string pattern() const override {
        return string(1, _character);
    }

private:
    const char _character;
};

class Choice : public Rule {
public:
    Choice(const vector<shared_ptr<Rule>> &rules) : _rules(rules) {}

    string pattern() const override {
        ostringstream oss;
        oss << "(?:";
        for (size_t i = 0; i < _rules.size(); i++) {
            oss << _rules[i]->pattern();
            if (i < _rules.size() - 1) {
                oss << "|";
            }
        }
        oss << ")";
        return oss.str();
    }

private:
    vector<shared_ptr<Rule>> _rules;
};

class Sequence : public Rule {
public:
    Sequence(const vector<shared_ptr<Rule>> &rules) : _rules(rules) {}

    string pattern() const override {
        ostringstream oss;
        oss << "(?:";
        for (const auto &rule : _rules) {
            oss << rule->pattern();
        }
        oss << ")";
        return oss.str();
    }

private:
    vector<shared_ptr<Rule>> _rules;
};

class Rule8 : public Rule {
public:
    explicit Rule8(const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> &collection) : _collection(collection) {}

    string pattern() const override {
        return "(?:" + _collection->at(42)->pattern() + "+)";
    }

private:
    const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> _collection;
};
class Rule11 : public Rule {
public:
    Rule11(const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> &collection) : _collection(collection) {}

    string pattern() const override {
        const string p42 = _collection->at(42)->pattern();
        const string p31 = _collection->at(31)->pattern();

        ostringstream oss;
        oss << "(";

        for (int i = 1; i < recursionLevel; i++) {
            ostringstream curly;
            curly << "{" << i << "}";

            ostringstream exact;
            oss << "(" << p42 << curly.str() << p31 << curly.str() << ")";

            if (i < recursionLevel - 1) {
                oss << "|";
            }
        }
        oss << ")";
        return oss.str();
    }

private:
    const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> _collection;
};
class Proxy : public Rule {
public:
    Proxy(const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> &collection, size_t index) : _collection(collection),
    _index(index) {}

    string pattern() const override {
        auto realRule = _collection->at(_index);
        return realRule->pattern();
    }

    size_t index() const {
        return _index;
    }

private:
    size_t _index;

    const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> _collection;
};

struct Input {
    const shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> rules = make_shared<unordered_map<size_t, shared_ptr<Rule>>>();
    vector<string> lines;
};

pair<size_t, shared_ptr<Rule>> parseRule(const string &line, shared_ptr<unordered_map<size_t, shared_ptr<Rule>>> ruleCollection) {
    auto index_pattern = split(line, ": ");

    size_t index = stoul(index_pattern[0]);
    string pattern = index_pattern[1];

    shared_ptr<Rule> rule;

    auto parseSequence = [ruleCollection](const string &s) -> shared_ptr<Rule> {
        auto subpatterns = split(s, ' ');
        vector<shared_ptr<Rule>> sequence;
        transform(subpatterns.cbegin(), subpatterns.cend(), back_inserter(sequence), [ruleCollection](auto &s) {
            return make_shared<Proxy>(ruleCollection, stoul(s));
        });
        return make_shared<Sequence>(sequence);
    };

    if (pattern[0] == '"') {
        rule = make_shared<Literal>(pattern[1]);
    } else if (pattern.find('|') != string::npos) {
        auto subpatterns = split(pattern, " | ");
        vector<shared_ptr<Rule>> choice;
        transform(subpatterns.cbegin(), subpatterns.cend(), back_inserter(choice), parseSequence);
        rule = make_shared<Choice>(choice);
    } else {
        rule = parseSequence(pattern);
    }

    return {index, rule};
}

Input readInput() {
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
    sections.push_back(current);

    Input result{};
    result.lines = sections[1];
    for (const auto& line : sections[0]) {
        auto parsed = parseRule(line, result.rules);
        result.rules->insert({parsed.first, parsed.second});
    }
    return result;
}

int main() {
    Input input = readInput();
    (*input.rules)[8] = make_shared<Rule8>(input.rules);
    (*input.rules)[11] = make_shared<Rule11>(input.rules);
    auto rule0 = input.rules->at(0);
    cout << "Part 2: " << count_if(input.lines.cbegin(), input.lines.cend(), [rule0](auto line) { return rule0->matches(line); });
}