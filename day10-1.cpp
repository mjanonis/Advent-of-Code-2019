#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>

// Ray-tracing solution

float magnitude(std::pair<float, float>& vec)
{
    return std::sqrt(std::pow(vec.first, 2) + std::pow(vec.second, 2));
}

void normalizeVec(std::pair<float, float>& vec)
{
    float mag = magnitude(vec);
    vec.first /= mag;
    vec.second /= mag;
}

struct Ray {
    std::pair<int, int> v0;
    std::pair<float, float> dir;

    Ray(const std::pair<int, int>& p1, const std::pair<int, int>& p2)
    {
        v0 = p1;
        dir =
            std::pair<float, float>(p2.first - p1.first, p2.second - p1.second);
        normalizeVec(dir);
    }
};

// returns if an intersection occured and the t value
std::pair<bool, float> getIntersectionT(Ray& r, std::pair<int, int>& point)
{
    std::pair<bool, float> res{false, 0};

    if (point == r.v0) {
        return res;
    }

    if (std::abs(((point.first - r.v0.first) * r.dir.second -
                  (point.second - r.v0.second) * r.dir.first)) < 0.0001) {
        res.first = true;
    }

    if (res.first) {
        if (r.dir.first) {
            res.second = (point.first - r.v0.first) / r.dir.first;
        }
        else if (r.dir.second) {
            res.second = (point.second - r.v0.second) / r.dir.second;
        }
    }

    if (res.second < 0) {
        res.first = false;
    }

    return res;
}

// returns the index of the asteroid that was hit with the lowest t value
int bestHitAsteroid(Ray& r, std::vector<std::pair<int, int>>& asteroids)
{
    float t = INT32_MAX;
    std::pair<bool, float> intersection;
    int best_asteroid = -1;
    for (int i = 0; i < asteroids.size(); ++i) {
        intersection = getIntersectionT(r, asteroids[i]);
        if (intersection.first) {
            if (intersection.second < t) {
                t = intersection.second;
                best_asteroid = i;
            }
        }
    }
    return best_asteroid;
}

int main()
{
    std::ifstream input{"day10.in"};
    std::ofstream output{"day10-1.out"};

    std::vector<std::string> map;
    std::string tmp;
    std::map<std::pair<int, int>, int> visibleAsteroids;

    while (std::getline(input, tmp)) {
        map.push_back(tmp);
    }

    // X, Y pairs
    std::vector<std::pair<int, int>> asteroids;

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == '#') {
                asteroids.emplace_back(j, i);
            }
        }
    }

    for (int i = 0; i < asteroids.size(); ++i) {
        for (int j = i; j < asteroids.size(); ++j) {
            if (i == j) {
                continue;
            }
            Ray r(asteroids[i], asteroids[j]);
            int best_asteroid = bestHitAsteroid(r, asteroids);

            if (best_asteroid == j) {
                visibleAsteroids[asteroids[i]] += 1;
                visibleAsteroids[asteroids[j]] += 1;
            }
        }
    }

    auto max =
        std::max_element(visibleAsteroids.begin(), visibleAsteroids.end(),
                         [](const std::pair<std::pair<int, int>, int>& a,
                            const std::pair<std::pair<int, int>, int>& b) {
                             return a.second < b.second;
                         });

    output << max->second << std::endl;
}