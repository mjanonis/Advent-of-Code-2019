#include <fstream>
#include <tuple>
#include <vector>

struct Moon {
    Moon() = default;
    Moon(std::tuple<int, int, int> p) : pos{p} {}
    std::tuple<int, int, int> pos;
    std::tuple<int, int, int> vel{0, 0, 0};
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
    std::ofstream output{"day12-1.out"};

    std::vector<Moon> moons;
    std::string tmp;
    int t_x, t_y, t_z;

    while (std::getline(input, tmp)) {
        std::sscanf(tmp.c_str(), "<x=%d, y=%d, z=%d>", &t_x, &t_y, &t_z);
        moons.emplace_back(std::tuple<int, int, int>(t_x, t_y, t_z));
    }

    for (int time = 0; time < 1000; ++time) {
        gravity(moons);
        velocity(moons);
    }

    int total_energy = 0;
    for (auto& m : moons) {
        total_energy += getPotentialEnergy(m) * getKineticEnergy(m);
    }
    output << total_energy;
}