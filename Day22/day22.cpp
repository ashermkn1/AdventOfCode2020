#include <iostream>
#include <fstream>
#include <deque>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
using namespace std;
size_t score(deque<int> deck) {
    size_t score = 0;
    while (!deck.empty()) {
        int card = deck.front();
        score += card * deck.size();
        deck.pop_front();
    }
    return score;
}
// false is player1 win, true is player2 win
pair<bool, size_t> recursive_combat(deque<int> player1, deque<int> player2) {
    int round = 1;
    static int total_games = 1;
    static int game = 1;
    cout << endl << "=== Game " << game << " ===" << endl;
    set<pair<deque<int>, deque<int>>> cache;
    bool two_won;
    while (!player1.empty() && !player2.empty()) {

        cout << endl << "-- Round " << round << " (Game " << game << ") --" << endl;
        cout << "Player 1's deck: ";
        copy(player1.begin(), player1.end(), ostream_iterator<int>(cout, ", "));
        cout << endl;

        cout << "Player 2's deck: ";
        copy(player2.begin(), player2.end(), ostream_iterator<int>(cout, ", "));
        cout << endl;

        int card1 = player1.front();
        int card2 = player2.front();

        cout << "Player 1 plays: " << card1 << endl;
        cout << "Player 2 plays: " << card2 << endl;
        // avoid infinite loops
        if (!cache.insert({player1, player2}).second) {
            cout << "Already seen: " << card1 << " vs. " << card2 << endl;
            cout << total_games << endl;
            return {false, score(player1)};
        }

        player1.pop_front();
        player2.pop_front();

        if (player1.size() >= card1 && player2.size() >= card2) {
            // create new decks for subgame
            deque<int> deck1;
            for (int i = 0; i < card1; i++) {
                deck1.push_back(player1[i]);
            }
            deque<int> deck2;
            for (int i = 0; i < card2; i++) {
                deck2.push_back(player2[i]);
            }

            cout << "Playing a sub-game to determine the winner..." << endl;
            total_games++;
            game++;
            two_won = recursive_combat(deck1, deck2).first;
            game--;
        } else {
            two_won = card2 > card1;
        }

        if (two_won) {
            player2.push_back(card2);
            player2.push_back(card1);
        } else {
            player1.push_back(card1);
            player1.push_back(card2);
        }
        cout << "Player " << two_won + 1 << " wins round " << round << " of game " << game << "!" << endl;
        round++;
    }
    auto& winner = player1.empty()? player2 : player1;
    cout << "The winner of game " << game << " is player " << player1.empty() + 1 << "!" << endl << endl;
    if (game != 1) {
        cout << "...anyway, back to game " << game - 1 << '.' << endl;
    } else {
        cout << endl << "== Post-game results ==" << endl;
        cout << "Player 1's deck: ";
        copy(player1.begin(), player1.end(), ostream_iterator<int>(cout, ", "));
        cout << endl;
        cout << "Player 2's deck: ";
        copy(player2.begin(), player2.end(), ostream_iterator<int>(cout, ", "));
        cout << endl;
        cout << total_games << endl;
    }
    return {player1.empty(), score(winner)};
}
int main() {
    ifstream reader("input.txt");

    vector<string> lines;
    string buffer;
    while (getline(reader, buffer)) {
        lines.push_back(buffer);
    }

    vector<vector<int>> sections;
    vector<int> current;
    for (const auto& line : lines) {
        if (line.empty()) {
            sections.push_back(current);
            current.clear();
        } else {
            current.push_back(stoi(line));
        }
    }
    if (!current.empty()) {
        sections.push_back(current);
    }

    deque<int> player1(sections[0].begin(), sections[0].end());
    deque<int> player2(sections[1].begin(), sections[1].end());
    deque<int> player12(player1);
    deque<int> player22(player2);

    while (!player1.empty() && !player2.empty()) {
        int card1 = player1.front();
        player1.pop_front();
        int card2 = player2.front();
        player2.pop_front();
        if (card1 > card2) {
            player1.push_back(card1);
            player1.push_back(card2);
        } else {
            player2.push_back(card2);
            player2.push_back(card1);
        }
    }

    auto& winner = player1.empty()? player2 : player1;
    cout << "Part 1: " << score(winner) << endl;

    cout << "Part 2: " << recursive_combat(player12,player22).second << endl;
}