#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <string>
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
            if (!ready_for_input) {
                ready_for_input = true;
                return 2;
            }
            getArgument(program, ip, 1, mode_first, rel_base) = input;
            ip += 2;
            ready_for_input = false;
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

bool inBeam(std::vector<long long int> program, std::pair<int, int> xy)
{
    int inpt = 0;
    int ip = 0;
    int rel_base = 0;
    int otpt = 0;
    bool rfi = false;
    int status = 0;

    int coord[2] = {xy.first, xy.second};
    int i = 0;

    while ((status = runIntCode(program, ip, inpt, otpt, rel_base, rfi))) {
        if (rfi) {
            inpt = coord[i];
            ++i;
        }
    }

    return otpt;
}

int main()
{
    std::ifstream input{"day19.in"};
    std::ofstream output{"day19-2.out"};

    std::vector<long long int> program;
    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoll(tmp));
    }

    // add more empty space
    program.resize(program.size() + 10000, 0);

    int x = 0, y = 0;

    // iterate until the top left and bottom right corners of the square are in
    // the beam
    while (!inBeam(program, {x + 99, y})) {
        ++y;
        if (!inBeam(program, {x, y + 99})) {
            ++x;
        }
    }
    output << x * 10000 + y;
}