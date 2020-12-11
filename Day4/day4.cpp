#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;
bool byr(const string& passport);

bool iyr(const string& passport);

bool eyr(const string& passport);

bool hgt(const string& passport);

bool hcl(const string& passport);

bool ecl(const string& passport);

bool pid(const string& passport);


bool isBlankLine(char const* line)
{
    for ( char const* cp = line; *cp; ++cp )
    {
        if ( !isspace(*cp) ) return false;
    }
    return true;
}

bool isBlankLine(string const& line)
{
    return isBlankLine(line.c_str());
}
int main() {
    ifstream reader("example.txt");
    int valid = 0;
    string buffer;
    string passport;
    while (getline(reader, buffer)) {
        if (!isBlankLine(buffer)) {
            passport += buffer;
        } else {
            if (byr(passport) &&
                iyr(passport) &&
                eyr(passport) &&
                hgt(passport) &&
                hcl(passport) &&
                ecl(passport) &&
                pid(passport))
            {
                valid++;
            }
            passport.clear();
        }
    }
    cout << valid;
}
bool pid(const string& passport) {
    const char *r = strstr(passport.c_str(), "pid:");
    if (!r) return false;

    int num = 0;
    sscanf(r+4,"%*d%n", &num);
    return num == 9;
}

bool ecl(const string& passport) {
    const char *r = strstr(passport.c_str(), "ecl:");
    if (!r) return false;
    if (!(isspace(r[7]))) return false;

    char *eyes[7] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    for (int i = 0; i < 7; i++)
    {
        if (strncmp(r+4, eyes[i], 3) == 0)
            return true;
    }
    return false;
}

bool hcl(const string& passport) {
    const char *r = strstr(passport.c_str(), "hcl:");
    if (!r) return false;

    int num = 0;
    sscanf(r+4, "#%*x%n", &num);
    return num == 7;
}

bool hgt(const string& passport) {
    const char *r = strstr(passport.c_str(), "hgt:");
    if (!r) return false;

    int h;
    char c, c1;

    if (sscanf(r+4, "%d%c%c", &h, &c, &c1) != 3)  return false;

    if (c == 'c' && c1 == 'm')
        return (h >= 150 && h <= 193);

    if (c == 'i' && c1 == 'n')
        return (h >= 59 && h <= 76);

    return false;
}

bool eyr(const string& passport) {
    const char* r = strstr(passport.c_str(), "eyr:");
    if (!r) return false;

    int yr = atoi(r+4);
    return yr >= 2020 && yr<=2030;
}

bool iyr(const string& passport) {
    const char *r = strstr(passport.c_str(), "iyr:");
    if (!r) return false;

    int yr = atoi(r+4);
    return yr >= 2010 && yr <= 2020;
}

bool byr(const string& passport) {
    const char *r = strstr(passport.c_str(), "byr:");
    if (!r) return false;

    int yr = atoi(r+4);
    return yr >= 1920 && yr <= 2002;
}

