#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

int main()
{
    std::ifstream input{"day3.in"};
    std::ofstream output{"day3-1.out"};

    std::vector<std::set<std::pair<int, int>>> wires;
    std::set<std::pair<int, int>> intersections;
    std::unordered_map<char, int> dir_X = {
        {'L', -1}, {'R', 1}, {'U', 0}, {'D', 0}};
    std::unordered_map<char, int> dir_Y = {
        {'L', 0}, {'R', 0}, {'U', 1}, {'D', -1}};

    std::string tmp;

    while (std::getline(input, tmp)) {
        std::istringstream iss{tmp};
        std::set<std::pair<int, int>> wire;
        int x = 0;
        int y = 0;
        while (std::getline(iss, tmp, ',')) {
            char dir = tmp[0];
            int mag = std::stoi(tmp.substr(1, tmp.size()));

            for (int i = 0; i < mag; ++i) {
                x += dir_X[dir];
                y += dir_Y[dir];
                std::pair<int, int> point{x, y};
                wire.insert(point);
            }
        }
        wires.push_back(wire);
    }

    std::set_intersection(wires[0].begin(), wires[0].end(), wires[1].begin(),
                          wires[1].end(),
                          std::inserter(intersections, intersections.begin()));

    int lowest_dist =
        abs(intersections.begin()->first) + abs(intersections.begin()->second);

    for (auto p : intersections) {
        int dist = abs(p.first) + abs(p.second);
        if (dist < lowest_dist) {
            lowest_dist = dist;
        }
    }
    output << lowest_dist;
}