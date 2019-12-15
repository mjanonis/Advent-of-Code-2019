#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

long long int oreRequirement(
    std::map<std::string, std::pair<std::vector<std::pair<int, std::string>>,
                                    int>>& rev_reaction,
    long long int fuel)
{
    std::map<std::string, long long int> require;
    require["FUEL"] = fuel;
    bool end = false;

    while (!end) {
        end = true;
        for (auto kv : require) {
            if (kv.second <= 0) {
                continue;
            }
            long long int times =
                std::ceil(static_cast<double>(kv.second) / rev_reaction[kv.first].second);
            for (auto react : rev_reaction[kv.first].first) {
                end = false;
                require[react.second] += times * react.first;
            }
            require[kv.first] -= times * rev_reaction[kv.first].second;
        }
    }
    return require["ORE"];
}

int main()
{
    std::ifstream input{"day14.in"};
    std::ofstream output{"day14-2.out"};

    std::string tmp;

    std::map<std::string,
             std::pair<std::vector<std::pair<int, std::string>>, int>>
        rev_reaction;

    std::map<std::string, long long int> require;

    while (std::getline(input, tmp)) {
        std::string from;
        std::string to;
        std::vector<std::pair<int, std::string>> tmp_v;
        std::pair<std::vector<std::pair<int, std::string>>, int> tmp_p;
        from = tmp.substr(0, tmp.find("=>") - 1);
        to = tmp.substr(tmp.find("=>") + 3, tmp.size());
        std::istringstream iss(from);
        while (std::getline(iss, tmp, ',')) {
            std::pair<int, std::string> req;
            if (tmp[0] == ' ') {
                tmp = tmp.substr(1, tmp.size());
            }
            std::string num = tmp.substr(0, tmp.find(' '));
            req.first = std::stoi(num);
            req.second = tmp.substr(tmp.find(' ') + 1, tmp.size());
            tmp_v.push_back(req);
        }
        auto res = std::make_pair(tmp_v, std::stoi(to.substr(0, to.find(' '))));

        rev_reaction[to.substr(to.find(' ') + 1, to.size())] = res;
    }

    // generate bounds for binary search

    long long int lower = 1, upper = 2;
    while (oreRequirement(rev_reaction, upper) < 1e12) {
        lower = upper;
        upper *= 2;
    }

    // binary search for the correct amount

    while (upper - lower > 1) {
        long long int half = lower + (upper - lower) / 2;
        if (oreRequirement(rev_reaction, half) > 1e12) {
            upper = half;
        }
        else {
            lower = half;
        }
    }
    output << lower;
}