#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
using namespace std;


int main() {
    ifstream reader("input");

    string buffer, form;
    int num = 0;
    int first = 0;
    int second = 0;

    while (getline(reader, buffer)) {
        if (!buffer.empty()) {
            num++;
            form += buffer;
        } else {
            unordered_set<char> questions(form.begin(), form.end());
            first += questions.size();
            for (int i = 0; i < 26; i++) {
                if (count(form.begin(), form.end(), i+'a') == num)
                    second++;
            }
            form.clear();
            num = 0;
        }
    }
    if (!form.empty()) {
        unordered_set<char> questions(form.begin(), form.end());
        first += questions.size();
        for (int i = 0; i < 26; i++) {
            if (count(form.begin(), form.end(), i+'a') == num)
                second++;
        }
    }
    cout << first << endl;
    cout << second;
}