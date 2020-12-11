#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

int calcseat(const string& line);


int main() {
    ifstream reader("input");
    int seats[1024] = {0};
    string line;
    int maxid;
    while (reader >> line) {
        int seatid = calcseat(line);
        maxid = max(maxid, seatid);
        seats[seatid] = 1;
    }
    cout << maxid << endl;

    for (int i = 1; i < maxid - 1; i++) {
        if (seats[i] == 0 && seats[i-1] == 1 && seats[i+1] == 1)
            cout << i;
    }
}

int calcseat(const string& line) {
    int id = 0;
    for (int i = 0; i < 10; i++) {
        id <<= 1;
        if ((line[i] & 4) == 0) {
            id++;
        }
    }
    return id;
}
