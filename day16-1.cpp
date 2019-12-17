#include <fstream>
#include <vector>

int patternValue(int digit_index, const std::vector<int>& pattern,
                 int& pattern_counter, int& times_repeated)
{
    int val = pattern[pattern_counter];
    ++times_repeated;

    if (times_repeated == digit_index + 1) {
        ++pattern_counter;
        if (pattern_counter == pattern.size()) {
            pattern_counter = 0;
        }
        times_repeated = 0;
    }
    return val;
}

int applyPattern(const std::vector<int>& digits, int digit_index,
                 const std::vector<int>& pattern)
{
    int pc = 0;
    int tr = 0;
    int result = 0;

    // throw away first value
    patternValue(digit_index, pattern, pc, tr);

    for (int i = 0; i < digits.size(); ++i) {
        result += digits[i] * patternValue(digit_index, pattern, pc, tr);
    }
    return abs(result) % 10;
}

int main()
{
    std::ifstream input{"day16.in"};
    std::ofstream output{"day16-1.out"};

    std::string tmp;
    std::vector<int> digits;
    std::vector<int> new_digits;
    std::vector<int> pattern{0, 1, 0, -1};
    while (std::getline(input, tmp)) {
        for (auto c : tmp) {
            digits.push_back(c - '0');
        }
    }

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < digits.size(); ++j) {
            new_digits.push_back(applyPattern(digits, j, pattern));
        }
        digits = new_digits;
        new_digits.clear();
    }

    for (int i = 0; i < 8; ++i) {
        output << digits[i];
    }
}