#include <algorithm>
#include <fstream>
#include <set>
#include <tuple>
#include <vector>
#include <numeric>

// Using tuples was a mistake

struct Moon {
    Moon() = default;
    Moon(std::tuple<int, int, int> p) : pos{p} {}
    std::tuple<int, int, int> pos;
    std::tuple<int, int, int> vel{0, 0, 0};

    std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>> getState()
    {
        return {pos, vel};
    }
};

void gravity(std::vector<Moon>& moons)
{
    for (int i = 0; i < moons.size(); ++i) {
        for (int j = i; j < moons.size(); ++j) {
            if (i == j) {
                continue;
            }
            if (std::get<0>(moons[i].pos) > std::get<0>(moons[j].pos)) {
                std::get<0>(moons[i].vel) -= 1;
                std::get<0>(moons[j].vel) += 1;
            }
            else if (std::get<0>(moons[i].pos) < std::get<0>(moons[j].pos)) {
                std::get<0>(moons[i].vel) += 1;
                std::get<0>(moons[j].vel) -= 1;
            }

            if (std::get<1>(moons[i].pos) > std::get<1>(moons[j].pos)) {
                std::get<1>(moons[i].vel) -= 1;
                std::get<1>(moons[j].vel) += 1;
            }
            else if (std::get<1>(moons[i].pos) < std::get<1>(moons[j].pos)) {
                std::get<1>(moons[i].vel) += 1;
                std::get<1>(moons[j].vel) -= 1;
            }

            if (std::get<2>(moons[i].pos) > std::get<2>(moons[j].pos)) {
                std::get<2>(moons[i].vel) -= 1;
                std::get<2>(moons[j].vel) += 1;
            }
            else if (std::get<2>(moons[i].pos) < std::get<2>(moons[j].pos)) {
                std::get<2>(moons[i].vel) += 1;
                std::get<2>(moons[j].vel) -= 1;
            }
        }
    }
}

void velocity(std::vector<Moon>& moons)
{
    for (auto& m : moons) {
        std::get<0>(m.pos) += std::get<0>(m.vel);
        std::get<1>(m.pos) += std::get<1>(m.vel);
        std::get<2>(m.pos) += std::get<2>(m.vel);
    }
}

int getPotentialEnergy(Moon& m)
{
    return abs(std::get<0>(m.pos)) + abs(std::get<1>(m.pos)) +
           abs(std::get<2>(m.pos));
}

int getKineticEnergy(Moon& m)
{
    return abs(std::get<0>(m.vel)) + abs(std::get<1>(m.vel)) +
           abs(std::get<2>(m.vel));
}

int main()
{
    std::ifstream input{"day12.in"};
    std::ofstream output{"day12-2.out"};

    std::vector<Moon> moons;
    std::string tmp;
    int t_x, t_y, t_z;

    while (std::getline(input, tmp)) {
        std::sscanf(tmp.c_str(), "<x=%d, y=%d, z=%d>", &t_x, &t_y, &t_z);
        moons.emplace_back(std::tuple<int, int, int>(t_x, t_y, t_z));
    }

    std::vector<long long int> axis_period(3, 0);

    std::vector<std::set<std::vector<std::pair<int, int>>>> axis_states(3);

    std::vector<bool> checked(3, false);
    bool exit = true;

    for (int time = 0; time < INT32_MAX; ++time) {
        std::vector<std::vector<std::pair<int, int>>> xyz;
        std::vector<std::pair<int, int>> t_x;
        std::vector<std::pair<int, int>> t_y;
        std::vector<std::pair<int, int>> t_z;

        for (auto& m : moons) {
            auto [px, py, pz] = m.pos;
            auto [vx, vy, vz] = m.vel;
            t_x.push_back({px, vx});
            t_y.push_back({py, vy});
            t_z.push_back({pz, vz});
        }
        xyz = {t_x, t_y, t_z};
        for (int i = 0; i < axis_states.size(); ++i) {
            auto it = axis_states[i].insert(xyz[i]);
            if (!it.second && !checked[i]) {
                axis_period[i] = time;
                checked[i] = true;
            }
        }
        for (auto b : checked) {
            exit = exit && b;
        }
        if (exit) {
            break;
        }
        exit = true;

        gravity(moons);
        velocity(moons);
    }

    long long int result = 0;
    result = std::lcm(axis_period[0], axis_period[1]);
    result = std::lcm(result, axis_period[2]);
    output << result;
}