#include <algorithm>
#include <fstream>
#include <tuple>
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

int getArgument(std::vector<int>& program, int ip, int offset, int mode)
{
    if (mode == 0) {
        return program[program[ip + offset]];
    }
    return program[ip + offset];
}

int main()
{
    std::ifstream input{"day7.in"};
    std::ofstream output{"day7-1.out"};

    std::vector<int> program;

    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoi(tmp));
    }

    std::vector<int> orig_program{program};

    int ip = 0;
    std::vector<int> phase_setting = {0, 1, 2, 3, 4};
    int ctr = 0; // phase setting counter
    int inpt = 0;
    bool isFirstIns = true;
    int max_thrust = 0;

    while (std::next_permutation(phase_setting.begin(), phase_setting.end())) {
        for (int i = 0; i < 5; ++i) {
            while (ip < program.size()) {
                auto [mode_third, mode_second, mode_first, op] =
                    getModeAndOpcode(program[ip]);

                if (op == 1) {
                    program[program[ip + 3]] =
                        getArgument(program, ip, 1, mode_first) +
                        getArgument(program, ip, 2, mode_second);

                    ip += 4;
                }
                else if (op == 2) {
                    program[program[ip + 3]] =
                        getArgument(program, ip, 1, mode_first) *
                        getArgument(program, ip, 2, mode_second);

                    ip += 4;
                }
                else if (op == 3) {
                    if (isFirstIns) {
                        program[program[ip + 1]] = phase_setting[ctr];
                        ++ctr;
                        isFirstIns = false;
                    }
                    else {
                        program[program[ip + 1]] = inpt;
                    }
                    ip += 2;
                }
                else if (op == 4) {
                    inpt = getArgument(program, ip, 1, mode_first);
                    ip += 2;
                }
                else if (op == 5) {
                    if (getArgument(program, ip, 1, mode_first) != 0) {
                        ip = getArgument(program, ip, 2, mode_second);
                        continue;
                    }
                    ip += 3;
                }
                else if (op == 6) {
                    if (getArgument(program, ip, 1, mode_first) == 0) {
                        ip = getArgument(program, ip, 2, mode_second);
                        continue;
                    }
                    ip += 3;
                }
                else if (op == 7) {
                    if (getArgument(program, ip, 1, mode_first) <
                        getArgument(program, ip, 2, mode_second)) {
                        program[program[ip + 3]] = 1;
                    }
                    else {
                        program[program[ip + 3]] = 0;
                    }
                    ip += 4;
                }
                else if (op == 8) {
                    if (getArgument(program, ip, 1, mode_first) ==
                        getArgument(program, ip, 2, mode_second)) {
                        program[program[ip + 3]] = 1;
                    }
                    else {
                        program[program[ip + 3]] = 0;
                    }
                    ip += 4;
                }
                else if (op == 99) {
                    break;
                }
                else {
                    output << "UNKNOWN OPCODE";
                }
            }
            ip = 0;
            isFirstIns = true;
            program = orig_program;
        }
        ctr = 0;
        if (inpt > max_thrust) {
            max_thrust = inpt;
        }
        inpt = 0;
    }

    output << max_thrust;
}