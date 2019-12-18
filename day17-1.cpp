#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <vector>

std::tuple<int, int, int, int> getModeAndOpcode(int num)
{
    std::tuple<int, int, int, int> res;
    int opcode = num % 100;
    num /= 100;
    int mode_first = num % 10;
    int mode_second = (num / 10) % 10;
    int mode_third = (num / 100) % 10;
    res = std::make_tuple(mode_third, mode_second, mode_first, opcode);
    return res;
}

long long int& getArgument(std::vector<long long int>& program, int ip,
                           int offset, int mode, int rel_base = 0)
{
    if (mode == 0) {
        return program[program[ip + offset]];
    }
    else if (mode == 1) {
        return program[ip + offset];
    }
    else if (mode == 2) {
        return program[program[ip + offset] + rel_base];
    }
}

int runIntCode(std::vector<long long int>& program, int& ip, int& input,
               int& output, int& rel_base)
{
    while (ip < program.size()) {
        auto [mode_third, mode_second, mode_first, op] =
            getModeAndOpcode(program[ip]);

        if (op == 1) {
            getArgument(program, ip, 3, mode_third, rel_base) =
                getArgument(program, ip, 1, mode_first, rel_base) +
                getArgument(program, ip, 2, mode_second, rel_base);

            ip += 4;
        }
        else if (op == 2) {
            getArgument(program, ip, 3, mode_third, rel_base) =
                getArgument(program, ip, 1, mode_first, rel_base) *
                getArgument(program, ip, 2, mode_second, rel_base);

            ip += 4;
        }
        else if (op == 3) {
            getArgument(program, ip, 1, mode_first, rel_base) = input;
            ip += 2;
        }
        else if (op == 4) {
            output = getArgument(program, ip, 1, mode_first, rel_base);
            ip += 2;
            return 1;
        }
        else if (op == 5) {
            if (getArgument(program, ip, 1, mode_first, rel_base) != 0) {
                ip = getArgument(program, ip, 2, mode_second, rel_base);
                continue;
            }
            ip += 3;
        }
        else if (op == 6) {
            if (getArgument(program, ip, 1, mode_first, rel_base) == 0) {
                ip = getArgument(program, ip, 2, mode_second, rel_base);
                continue;
            }
            ip += 3;
        }
        else if (op == 7) {
            if (getArgument(program, ip, 1, mode_first, rel_base) <
                getArgument(program, ip, 2, mode_second, rel_base)) {
                getArgument(program, ip, 3, mode_third, rel_base) = 1;
            }
            else {
                getArgument(program, ip, 3, mode_third, rel_base) = 0;
            }
            ip += 4;
        }
        else if (op == 8) {
            if (getArgument(program, ip, 1, mode_first, rel_base) ==
                getArgument(program, ip, 2, mode_second, rel_base)) {
                getArgument(program, ip, 3, mode_third, rel_base) = 1;
            }
            else {
                getArgument(program, ip, 3, mode_third, rel_base) = 0;
            }
            ip += 4;
        }
        else if (op == 9) {
            rel_base += getArgument(program, ip, 1, mode_first, rel_base);
            ip += 2;
        }
        else if (op == 99) {
            return 0; // finished executing
        }
    }
    return -1; // something went wrong
}

std::string getNeighbours(const std::vector<std::string>& map,
                          std::pair<int, int> coord)
{
    std::string res = "";
    // UP, RIGHT, DOWN, LEFT
    int DX[4]{0, 1, 0, -1};
    int DY[4]{1, 0, -1, 0};

    for (int i = 0; i < 4; ++i) {
        std::pair<int, int> neigh = {coord.first + DX[i], coord.second + DY[i]};
        if ((neigh.first < 0 || neigh.second < 0) ||
            (neigh.first > map[0].size() || neigh.second > map.size())) {
            continue;
        }
        res.push_back(map[neigh.second][neigh.first]);
    }
    return res;
}

std::set<std::pair<int, int>>
getIntesections(const std::vector<std::string>& map)
{
    std::set<std::pair<int, int>> intersections;
    std::string neigh;
    int count = 0;
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            neigh = getNeighbours(map, {x, y});
            count = std::count(neigh.begin(), neigh.end(), '#');
            if (count == 4 && map[y][x] == '#') {
                intersections.insert({x, y});
            }
        }
    }
    return intersections;
}

int main()
{
    std::ifstream input{"day17.in"};
    std::ofstream output{"day17-1.out"};

    std::vector<std::string> map;

    std::vector<long long int> program;
    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoll(tmp));
    }

    // add more empty space
    program.resize(program.size() + 10000, 0);

    int inpt = 0;
    int ip = 0;
    int rel_base = 0;
    int otpt = 0;

    tmp = "";
    while (runIntCode(program, ip, inpt, otpt, rel_base)) {
        if (otpt == 10) {
            map.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp.push_back(char(otpt));
    }

    // delete the last line as it is empty
    map.pop_back();

    auto intersections = getIntesections(map);

    int sum = 0;
    for (auto p : intersections) {
        sum += p.first * p.second;
    }
    output << sum;
}