#include <fstream>
#include <map>
#include <set>

void countOrbits(std::map<std::string, std::set<std::string>>& orbitedBy,
                 std::string planet, std::map<std::string, int>& orbitCount,
                 int oc)
{
    orbitCount[planet] = oc;
    for (auto x : orbitedBy[planet]) {
        countOrbits(orbitedBy, x, orbitCount, oc + 1);
    }
}

int main()
{
    std::ifstream input{"day6.in"};
    std::ofstream output{"day6-1.out"};

    std::map<std::string, std::set<std::string>> orbitedBy;
    std::map<std::string, int> orbitCount;
    std::string tmp;

    while (std::getline(input, tmp)) {
        orbitedBy[tmp.substr(0, tmp.find(')'))].insert(
            tmp.substr(tmp.find(')') + 1, tmp.size()));
    }
    countOrbits(orbitedBy, "COM", orbitCount, 0);

    int oc = 0;

    for (auto x : orbitCount) {
        oc += x.second;
    }
    output << oc;
}