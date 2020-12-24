#pragma once

#include <vector>
#include <string>
#include <sstream>
std::vector<std::string> split(const std::string &original, char delimiter = ' ', int maxSplits = -1)
{
    std::vector<std::string> result;
    std::string buffer;
    std::istringstream source(original);
    int splits = 0;

    while (getline(source, buffer, delimiter) && splits != maxSplits)
    {
        result.push_back(buffer);
        splits++;
    }
    return result;
}

std::vector<std::string> split(const std::string &original, const std::string &delimiter, int maxSplits = -1)
{
    std::vector<std::string> result;
    int splits = 0;
    size_t offset = 0;

    while (true)
    {
        if (splits == maxSplits)
        {
            break;
        }
        size_t pos = original.find(delimiter, offset);
        if (pos == std::string::npos)
        {
            break;
        }
        result.push_back(original.substr(offset, pos - offset));
        offset = pos + delimiter.length();
    }
    result.push_back(original.substr(offset));
    return result;
}