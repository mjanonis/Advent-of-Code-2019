#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

int main()
{
    std::ifstream input{"day3.in"};
    std::ofstream output{"day3-2.out"};

    std::vector<std::set<std::pair<int, int>>> wires;
    std::set<std::pair<int, int>> intersections;
    std::unordered_map<char, int> dir_X = {
        {'L', -1}, {'R', 1}, {'U', 0}, {'D', 0}};
    std::unordered_map<char, int> dir_Y = {
        {'L', 0}, {'R', 0}, {'U', 1}, {'D', -1}};

    std::vector<std::map<std::pair<int, int>, int>> points_to_steps;

    std::string tmp;

    while (std::getline(input, tmp)) {
        std::istringstream iss{tmp};
        std::set<std::pair<int, int>> wire;
        std::map<std::pair<int, int>, int> pts;
        int x = 0;
        int y = 0;
        int steps = 0;
        while (std::getline(iss, tmp, ',')) {
            char dir = tmp[0];
            int mag = std::stoi(tmp.substr(1, tmp.size()));

            for (int i = 0; i < mag; ++i) {
                x += dir_X[dir];
                y += dir_Y[dir];
                steps += 1;
                std::pair<int, int> point{x, y};
                wire.insert(point);
                pts.insert({point, steps});
            }
        }
        points_to_steps.push_back(pts);
        wires.push_back(wire);
    }

    std::set_intersection(wires[0].begin(), wires[0].end(), wires[1].begin(),
                          wires[1].end(),
                          std::inserter(intersections, intersections.begin()));

    int lowest_steps = points_to_steps[0][*intersections.begin()] +
                       points_to_steps[1][*intersections.begin()];

    for (auto p : intersections) {
        int steps = points_to_steps[0][p] + points_to_steps[1][p];
        if (steps < lowest_steps) {
            lowest_steps = steps;
        }
    }
    output << lowest_steps;
}