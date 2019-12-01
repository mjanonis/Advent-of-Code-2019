#include <iostream>
#include <fstream>

int main()
{
    std::ifstream input {"day1.in"};
    std::ofstream output {"day1-1.out"};
    int mass;
    int fuel_req = 0;
    while(input >> mass){
        fuel_req += mass / 3 - 2;
    }

    output << fuel_req;
}