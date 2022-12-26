#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <map>
#include <set>
#include "../common.h"
using namespace std;


int main() {
    ifstream reader("input.txt");
    // maps allergens to list of ingredients
    map<string, set<string>> allergens;
    multiset<string> safe_ingredients;

    string line;
    while (getline(reader, line)) {
        // split into allergens and ingredients
        auto tokens = split(line, " (contains ");
        set<string> ingredients;
        // add all ingredients
        for (const auto& ingredient : split(tokens[0])) {
            ingredients.insert(ingredient);
            safe_ingredients.insert(ingredient);
        }

        for (const auto& allergen : split(tokens[1].substr(0, tokens[1].size() - 1), ", ")) {
            auto [elem, is_new] = allergens.emplace(allergen, ingredients);
            // if allergen already encountered
            if (!is_new) {
                // possible ingredients is intersection of two sets
                set<string> intersection;
                set_intersection(ingredients.begin(), ingredients.end(),
                                 elem->second.begin(), elem->second.end(),
                                 inserter(intersection, intersection.begin()));

                elem->second = move(intersection);
            }
        }
    }
    // erase all possible allergen causers
    for (const auto& kv : allergens) {
        for (const auto& ingredient : kv.second) {
            safe_ingredients.erase(ingredient);
        }
    }
    cout << "Part 1: " << safe_ingredients.size() << endl;

    while (any_of(allergens.begin(), allergens.end(), [](const auto& kv){return kv.second.size() > 1;})) {
        for (auto& [allergen, possibles] : allergens) {
            if (possibles.size() == 1) {
                string ingredient = *possibles.begin();
                for (auto& other : allergens) {
                    if (allergen != other.first) {
                        other.second.erase(ingredient);
                    }
                }
            }
        }
    }
    cout << "Part 2: ";
    for (const auto& c : allergens) {
        cout << *c.second.begin() << ",";
    }
    cout << endl;
}