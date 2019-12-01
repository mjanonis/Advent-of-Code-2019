#include <iostream>
#include <fstream>

int main()
{
    std::ifstream input{"day1.in"};
    std::ofstream output{"day1-2.out"};
    int mass;
    int fuel_req = 0;
    int fuel_mod_req = 0;
    while (input >> mass)
    {
        fuel_mod_req = mass / 3 - 2;
        fuel_req += fuel_mod_req;
        while (fuel_mod_req >= 0)
        {
            fuel_mod_req = fuel_mod_req / 3 - 2;
            if (fuel_mod_req > 0)
            {
                fuel_req += fuel_mod_req;
            }
        }
    }

    output << fuel_req;
}