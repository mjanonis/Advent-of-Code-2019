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
    std::ifstream input{"day5.in"};
    std::ofstream output{"day5-1.out"};

    std::vector<int> program;

    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoi(tmp));
    }

    int ip = 0;
    int inpt = 1;

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
            program[program[ip + 1]] = inpt;
            ip += 2;
        }
        else if (op == 4) {
            output << getArgument(program, ip, 1, mode_first) << std::endl;
            ip += 2;
        }
        else if (op == 99) {
            break;
        }
        else {
            output << "UNKNOWN OPCODE";
        }
    }
}