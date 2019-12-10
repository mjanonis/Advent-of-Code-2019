#include <fstream>
#include <vector>

int main()
{
    std::ifstream input{"day2.in"};
    std::ofstream output{"day2-1.out"};

    std::vector<int> program;

    std::string tmp;
    while (std::getline(input, tmp, ','))
    {
        program.push_back(std::stoi(tmp));
    }

    program[1] = 12;
    program[2] = 2;

    int ip = 0;
    while (ip < program.size())
    {
        // std::cout << program[ip] << std::endl;
        if (program[ip] == 1)
        {
            program[program[ip + 3]] = program[program[ip + 1]] + program[program[ip + 2]];
        }
        else if (program[ip] == 2)
        {
            program[program[ip + 3]] = program[program[ip + 1]] * program[program[ip + 2]];
        }
        else if (program[ip] == 99)
        {
            break;
        }
        else
        {
            output << "UNKNOWN OPCODE";
        }
        ip += 4;
    }
    output << program[0];
}
