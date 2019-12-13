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

int main()
{
    std::ifstream input{"day13.in"};
    std::ofstream output{"day13-2.out"};

    std::vector<long long int> program;
    std::string tmp;
    while (std::getline(input, tmp, ',')) {
        program.push_back(std::stoll(tmp));
    }

    // add more empty space
    program.resize(program.size() + 10000, 0);

    // play for free
    program[0] = 2;

    int inpt = 0;
    int ip = 0;
    int rel_base = 0;
    int otpt = 0;
    int status = 0;

    int x = 0;
    int y = 0;
    int tile = 0;
    int score = 0;

    int get_score = 0;

    std::pair<int, int> ball_coord;
    std::pair<int, int> paddle_coord;

    while (runIntCode(program, ip, inpt, otpt, rel_base)) {
        x = otpt;
        status = runIntCode(program, ip, inpt, otpt, rel_base);
        y = otpt;
        if (x == -1 && y == 0) {
            ++get_score;
        }
        status = runIntCode(program, ip, inpt, otpt, rel_base);
        tile = otpt;

        if (get_score == 3) {
            score = otpt;
            get_score = 0;
        }

        if (tile == 4) {
            ball_coord.first = x;
            ball_coord.second = y;
        }
        if (tile == 3) {
            paddle_coord.first = x;
            paddle_coord.second = y;
        }

        // make sure the paddle is under the ball at all times

        if (paddle_coord.first < ball_coord.first) {
            inpt = 1;
        }
        if (paddle_coord.first > ball_coord.first) {
            inpt = -1;
        }
        if (paddle_coord.first == ball_coord.first) {
            inpt = 0;
        }
    }

    output << score;
}