#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// NOTE: I did the second part by hand, but I will update this file whenever
// I have the time to figure out a proper solution

// TODO: Implement LZ77/LZ78 compression

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
                           int offset, int mode, int rel_base)
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
               int& output, int& rel_base, bool& ready_for_input)
{
    ready_for_input = false;
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
            ready_for_input = true;
            getArgument(program, ip, 1, mode_first, rel_base) = input;
            ip += 2;
            return 2;
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

struct Robot {
    std::pair<int, int> pos;

    // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
    int dir = 0;
};

void turnRight(Robot& rbt) { rbt.dir = (rbt.dir + 1) % 4; }
void turnLeft(Robot& rbt)
{
    rbt.dir = (rbt.dir - 1) % 4;
    if (rbt.dir < 0) {
        rbt.dir = 4 + rbt.dir;
    }
}

char getDirection(const std::vector<std::string>& map, Robot& rbt)
{
    // UP, RIGHT, DOWN, LEFT
    int DX[4]{0, 1, 0, -1};
    int DY[4]{-1, 0, 1, 0};

    for (int i = 0; i < 4; ++i) {
        std::pair<int, int> neigh = {rbt.pos.first + DX[i],
                                     rbt.pos.second + DY[i]};
        if ((neigh.first < 0 || neigh.second < 0) ||
            (neigh.first > map[0].size() || neigh.second > map.size())) {
            continue;
        }
        if (map[neigh.second][neigh.first] == '#') {
            switch (rbt.dir) {
            case 0:
                // do not consider a node behind the robot
                if (rbt.pos.second != neigh.second) {
                    break;
                }
                if (neigh.first > rbt.pos.first) {
                    return 'R';
                }
                else {
                    return 'L';
                }
                break;
            case 1:
                // do not consider a node behind the robot
                if (rbt.pos.first != neigh.first) {
                    break;
                }
                if (neigh.second > rbt.pos.second) {
                    return 'R';
                }
                else {
                    return 'L';
                }
                break;
            case 2:
                // do not consider a node behind the robot
                if (rbt.pos.second != neigh.second) {
                    break;
                }
                if (neigh.first < rbt.pos.first) {
                    return 'R';
                }
                else {
                    return 'L';
                }
                break;
            case 3:
                // do not consider a node behind the robot
                if (rbt.pos.first != neigh.first) {
                    break;
                }
                if (neigh.second < rbt.pos.second) {
                    return 'R';
                }
                else {
                    return 'L';
                }
                break;
            }
        }
    }
    return 'X';
}

bool endOfPath(const std::vector<std::string>& map, std::pair<int, int> coord)
{
    if (coord.first >= map[0].size() || coord.first < 0) {
        return true;
    }
    if (coord.second >= map.size() || coord.second < 0) {
        return true;
    }
    if (map[coord.second][coord.first] == '.') {
        return true;
    }
    return false;
}

int main()
{
    std::ifstream input{"day17.in"};
    std::ofstream output{"day17-2.out"};

    std::vector<std::string> map;

    std::vector<long long int> program;
    std::vector<long long int> old_program;
    std::vector<char> path;
    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoll(tmp));
    }

    // add more empty space
    program.resize(program.size() + 10000, 0);
    old_program = program;

    int inpt = 0;
    int ip = 0;
    int rel_base = 0;
    int otpt = 0;
    bool rfi = false;

    tmp = "";
    while (runIntCode(program, ip, inpt, otpt, rel_base, rfi)) {
        if (otpt == 10) {
            map.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp.push_back(char(otpt));
    }

    // delete the last line as it is empty
    map.pop_back();

    Robot rbt;
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            if (map[y][x] == '^') {
                rbt.pos = {x, y};
            }
        }
    }

    // UP, RIGHT, DOWN, LEFT
    int DX[4]{0, 1, 0, -1};
    int DY[4]{-1, 0, 1, 0};

    char dir = '?';
    dir = getDirection(map, rbt);

    while (dir != 'X') {
        int ctr = 0;
        if (dir == 'R') {
            turnRight(rbt);
        }
        else {
            turnLeft(rbt);
        }
        while (!endOfPath(
            map, {rbt.pos.first + DX[rbt.dir], rbt.pos.second + DY[rbt.dir]})) {
            rbt.pos.first += DX[rbt.dir];
            rbt.pos.second += DY[rbt.dir];
            ++ctr;
        }
        path.push_back(dir);
        path.push_back(',');

        std::string number = std::to_string(ctr);
        for (char c : number) {
            path.push_back(c);
        }

        path.push_back(',');
        dir = getDirection(map, rbt);
    }

    path.pop_back();
    path.push_back(char(10));

    // ABBACACACB
    std::string main_mov = "A,B,B,A,C,A,C,A,C,B\n";
    std::string A = "L,6,R,12,R,8\n";
    std::string B = "R,8,R,12,L,12\n";
    std::string C = "R,12,L,12,L,4,L,4\n";
    std::string total_input = main_mov + A + B + C + "n\n";

    // reset the intcode machine
    program = old_program;
    ip = 0;
    rel_base = 0;

    // activate the robot
    program[0] = 2;

    int ctr = 0;
    inpt = 0;
    rfi = false;
    inpt = total_input[ctr];
    ++ctr;

    while (runIntCode(program, ip, inpt, otpt, rel_base, rfi)) {
        if (rfi) {
            if (ctr < total_input.size()) {
                inpt = total_input[ctr];
                ++ctr;
            }
        }
    }
    std::cout << otpt;
}