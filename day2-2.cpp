#include <fstream>
#include <vector>

int main()
{
    std::ifstream input{"day2.in"};
    std::ofstream output{"day2-2.out"};
    const int target = 19690720;
    std::vector<int> program;

    std::string tmp;
    while (std::getline(input, tmp, ','))
    {
        program.push_back(std::stoi(tmp));
    }

    std::vector<int> orig_program{program};

    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            program = orig_program;
            program[1] = i;
            program[2] = j;

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
            if (program[0] == target)
            {
                output << 100 * i + j << std::endl;
                return 0;
            }
        }
    }
}
