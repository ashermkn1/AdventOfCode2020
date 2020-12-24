#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    ifstream reader("input.txt");

    int x = 0;
    int y = 0;
    int wayX = 10;
    int wayY = 1;
    int x2 = 0;
    int y2 = 0;
    int angle = 90;
    string instruction;

    while (reader >> instruction) {
        int steps = stoi(instruction.substr(1));

        switch (instruction[0]) {
            case 'N':
                y += steps;
                wayY += steps;
                break;
            case 'E':
                x += steps;
                wayX += steps;
                break;
            case 'W':
                x -= steps;
                wayX -= steps;
                break;
            case 'S':
                y -= steps;
                wayY -= steps;
                break;
            case 'L':
                angle = (angle - steps + 360) % 360;

                for (int i = 0; i < steps; i+=90) {
                    swap(wayY, wayX);
                    wayX *= -1;
                }
                break;
            case 'R':
                angle = (angle + steps) % 360;
                for (int i = 0; i < steps; i+=90) {
                    swap(wayY, wayX);
                    wayY *= -1;
                }
                break;
            case 'F':
                switch (angle) {
                    case 0:
                        y += steps;
                        break;
                    case 90:
                        x += steps;
                        break;
                    case 180:
                        y -= steps;
                        break;
                    case 270:
                        x -= steps;
                        break;
                }
                x2 += steps * wayX;
                y2 += steps * wayY;
                break;
        }
        cout << "(" << x2 << ", " << y2 << ")" << endl;
        cout << "(" << wayX << ", " << wayY << ")" << endl << endl;
    }
    cout << "Part 1: " << abs(x) + abs(y) << endl;
    cout << "Part 2: " << abs(x2) + abs(y2) << endl;
}