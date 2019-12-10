#include <fstream>
#include <map>
#include <set>

int main()
{
    std::ifstream input{"day6.in"};
    std::ofstream output{"day6-2.out"};

    std::map<std::string, std::string> dirOrbits;
    std::map<std::string, bool> visited;
    std::string tmp;
    std::string lca; // lowest commmon ancestor
    int dist = 0;

    while (std::getline(input, tmp)) {
        dirOrbits[tmp.substr(tmp.find(')') + 1, tmp.size())] =
            tmp.substr(0, tmp.find(')'));
    }

    // find the lowest common ancestor node
    tmp = "YOU";
    while (dirOrbits[tmp] != "COM") {
        visited[tmp] = true;
        tmp = dirOrbits[tmp];
    }

    tmp = "SAN";
    while (dirOrbits[tmp] != "COM") {
        if (visited[tmp]) {
            lca = tmp;
            break;
        }
        visited[tmp] = true;
        tmp = dirOrbits[tmp];
        ++dist;
    }

    // get the distance to the lca
    tmp = "YOU";
    while (dirOrbits[tmp] != lca) {
        tmp = dirOrbits[tmp];
        ++dist;
    }

    // take one away as we counted the lca node twice
    output << dist - 1;
}