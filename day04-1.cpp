#include <algorithm>
#include <fstream>
#include <vector>

bool noDecreasingDigits(std::string num)
{
    std::string orig{num};
    std::sort(num.begin(), num.end());
    return num == orig;
}

bool adjDigitsAreSame(std::string num)
{
    return num[0] == num[1] || num[1] == num[2] || num[2] == num[3] ||
           num[3] == num[4] || num[4] == num[5];
}

int main()
{
    std::ifstream input{"day4.in"};
    std::ofstream output{"day4-1.out"};

    std::vector<std::string> passwords;
    std::string tmp;
    std::getline(input, tmp);
    int range_begin = std::stoi(tmp.substr(0, tmp.find('-')));
    int range_end = std::stoi(tmp.substr(tmp.find('-') + 1, tmp.length()));

    for (int i = range_begin; i <= range_end; ++i) {
        tmp = std::to_string(i);
        if (noDecreasingDigits(tmp) && adjDigitsAreSame(tmp)) {
            passwords.push_back(tmp);
        }
    }

    output << passwords.size();
}