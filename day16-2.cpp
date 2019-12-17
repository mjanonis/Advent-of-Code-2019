#include <cmath>
#include <fstream>
#include <vector>

long long int vectorSum(const std::vector<int>& v)
{
    long long int sum = 0;
    for (auto x : v) {
        sum += x;
    }
    return sum;
}

int main()
{
    std::ifstream input{"day16.in"};
    std::ofstream output{"day16-2.out"};

    std::string tmp;
    std::vector<int> digits;
    std::vector<int> new_digits;
    std::vector<int> pattern{0, 1, 0, -1};
    int message_offset = 0;

    while (std::getline(input, tmp)) {
        for (auto c : tmp) {
            digits.push_back(c - '0');
        }
    }

    new_digits = digits;
    for (int i = 1; i < 10000; ++i) {
        digits.insert(digits.end(), new_digits.begin(), new_digits.end());
    }

    int power = 6;
    for (int i = 0; i < 7; ++i) {
        message_offset += digits[i] * pow(10, power);
        --power;
    }

    // splice the list
    digits = std::vector<int>(digits.begin() + message_offset, digits.end());
    new_digits.clear();

    for (int i = 0; i < 100; ++i) {
        // the upper part of the triangular matrix is all 1's, so it becomes
        // simple addition
        long long int sum = vectorSum(digits);
        for (int j = 0; j < digits.size(); ++j) {
            new_digits.push_back(abs(sum) % 10);
            sum -= digits[j];
        }
        digits = new_digits;
        new_digits.clear();
    }

    for (int i = 0; i < 8; ++i) {
        output << digits[i];
    }
}