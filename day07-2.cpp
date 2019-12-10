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

int loopbackNext(int i)
{
    if (i + 1 == 5) {
        return 0;
    }
    return i + 1;
}

int loopbackPrev(int i)
{
    if (i == 0) {
        return 4;
    }
    return i - 1;
}

int runIntCode(std::vector<int>& program, int& ip, int& input, int& output,
               int& isFirstInput, int& phase_setting)
{
    while (ip < program.size()) {
        auto [mode_third, mode_second, mode_first, op] =
            getModeAndOpcode(program[ip]);

        if (op == 1) {
            program[program[ip + 3]] = getArgument(program, ip, 1, mode_first) +
                                       getArgument(program, ip, 2, mode_second);

            ip += 4;
        }
        else if (op == 2) {
            program[program[ip + 3]] = getArgument(program, ip, 1, mode_first) *
                                       getArgument(program, ip, 2, mode_second);

            ip += 4;
        }
        else if (op == 3) {
            if (isFirstInput) {
                program[program[ip + 1]] = phase_setting;
                isFirstInput = 0;
            }
            else {
                program[program[ip + 1]] = input;
            }
            ip += 2;
        }
        else if (op == 4) {
            output = getArgument(program, ip, 1, mode_first);
            ip += 2;
            return 1;
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
            return 0; // finished executing
        }
    }
    return -1; // something went wrong
}

int main()
{
    std::ifstream input{"day7.in"};
    std::ofstream output{"day7-2.out"};

    std::vector<int> program;

    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoi(tmp));
    }

    std::vector<int> phase_setting = {5, 6, 7, 8, 9};
    int ctr = 0; // phase setting counter

    std::vector<std::vector<int>> thrusters(5, program);
    std::vector<int> outputs(5, 0);
    std::vector<int> isFirstInput(5, 1);

    std::vector<int> ip(5, 0);

    int i = 0;
    int max_thrust = 0;

    while (std::next_permutation(phase_setting.begin(), phase_setting.end())) {
        while (runIntCode(thrusters[i], ip[i], outputs[loopbackPrev(i)],
                          outputs[i], isFirstInput[i], phase_setting[i])) {
            i = loopbackNext(i);
        }
        if (outputs[4] > max_thrust) {
            max_thrust = outputs[4];
        }
        // reset everything
        i = 0;
        std::fill(ip.begin(), ip.end(), 0);
        std::fill(outputs.begin(), outputs.end(), 0);
        std::fill(thrusters.begin(), thrusters.end(), program);
        std::fill(isFirstInput.begin(), isFirstInput.end(), 1);
    }
    output << max_thrust;
}