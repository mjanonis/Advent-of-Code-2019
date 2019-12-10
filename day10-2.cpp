#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>

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

    Ray() = default;

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

void sortByAngleClockwise(std::vector<Ray>& rays)
{
    std::sort(rays.begin(), rays.end(), [](Ray& r1, Ray& r2) {
        float ang1 = std::atan2(-r1.dir.second, -r1.dir.first);
        float ang2 = std::atan2(-r2.dir.second, -r2.dir.first);
        if (ang1 > M_PI / 2.0) {
            ang1 -= 2 * M_PI;
        }
        if (ang2 > M_PI / 2.0) {
            ang2 -= 2 * M_PI;
        }
        return ang1 < ang2;
    });
}

int main()
{
    std::ifstream input{"day10.in"};
    std::ofstream output{"day10-2.out"};

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

    std::pair<int, int> mon_station = max->first;
    Ray laser;
    laser.v0 = mon_station;
    laser.dir = std::make_pair(0, -1);

    int no_removed_asteroids = 0;
    std::vector<Ray> visible_from_station;
    std::pair<int, int> asteroid_coord;

    while (no_removed_asteroids != 200) {
        // find new visible asteroids from the station
        if (visible_from_station.empty()) {
            for (int i = 0; i < asteroids.size(); ++i) {
                Ray r(mon_station, asteroids[i]);
                int best_asteroid = bestHitAsteroid(r, asteroids);

                if (best_asteroid == i) {
                    visible_from_station.push_back(r);
                }
            }
            sortByAngleClockwise(visible_from_station);
        }

        int to_delete = bestHitAsteroid(
            visible_from_station[no_removed_asteroids], asteroids);
        asteroid_coord = asteroids[to_delete];
        asteroids.erase(asteroids.begin() + to_delete);
        ++no_removed_asteroids;
    }

    output << asteroid_coord.first * 100 + asteroid_coord.second;
}