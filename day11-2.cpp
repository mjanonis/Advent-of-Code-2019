#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

void turnRight(int& direction)
{
    if (direction == 3) {
        direction = 0;
        return;
    }
    direction += 1;
}

void turnLeft(int& direction)
{
    if (direction == 0) {
        direction = 3;
        return;
    }
    direction -= 1;
}

void turn(int& curr_dir, int instruction)
{
    if (instruction == 0) {
        turnLeft(curr_dir);
        return;
    }
    turnRight(curr_dir);
}

struct Robot {
    // X, Y
    std::pair<int, int> pos{0, 0};
    // 0 - UP; 1 - RIGHT; 2 - DOWN; 3 - LEFT
    int direction = 0;
};

int main()
{
    std::ifstream input{"day11.in"};
    std::ofstream output{"day11-2.out"};

    std::vector<long long int> program;
    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoll(tmp));
    }

    // add more empty space
    program.resize(program.size() + 10000, 0);

    int inpt = 1;
    int ip = 0;
    int rel_base = 0;
    int otpt = 0;
    int status = 0;
    std::pair<int, int> color_direction;
    std::set<std::pair<int, int>> painted_panels;
    std::map<std::pair<int, int>, int> panel_color;
    int DX[4] = {0, 1, 0, -1};
    int DY[4] = {1, 0, -1, 0};
    Robot rbt;

    while (runIntCode(program, ip, inpt, otpt, rel_base)) {
        color_direction.first = otpt;
        status = runIntCode(program, ip, inpt, otpt, rel_base);
        if (!status) {
            break;
        }
        color_direction.second = otpt;
        // paint the panel
        panel_color[rbt.pos] = color_direction.first;
        painted_panels.insert(rbt.pos);
        // turn the robot
        turn(rbt.direction, color_direction.second);
        // move the robot
        rbt.pos.first += DX[rbt.direction];
        rbt.pos.second += DY[rbt.direction];
        // read the new panel color
        inpt = panel_color[rbt.pos];
    }

    // get image dimensions
    int min_x = painted_panels.begin()->first;
    int max_x = 0;
    int min_y = painted_panels.begin()->second;
    int max_y = 0;

    for (auto& p : painted_panels) {
        if (p.first < min_x) {
            min_x = p.first;
        }
        if (p.first > max_x) {
            max_x = p.first;
        }
        if (p.second < min_y) {
            min_y = p.second;
        }
        if (p.second > max_y) {
            max_y = p.second;
        }
    }

    std::vector<std::vector<int>> identifier(
        abs(min_y) + abs(max_y) + 1,
        std::vector<int>(abs(min_x) + abs(max_x) + 1, 0));

    // flip the image on the horizontal axis

    for (auto& p : painted_panels) {
        identifier[p.second * -1][p.first + abs(min_x)] = panel_color[p];
    }

    // output the picture

    for (size_t y = 0; y < identifier.size(); ++y) {
        for (size_t x = 0; x < identifier[y].size(); ++x) {
            output << identifier[y][x];
        }
        output << std::endl;
    }
}