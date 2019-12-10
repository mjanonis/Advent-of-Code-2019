#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

void handleInput(std::ifstream& input,
                 std::vector<std::vector<std::vector<int>>>& image,
                 const int width, const int height)
{
    char tmp;
    while (!input.eof()) {
        std::vector<std::vector<int>> img;
        for (int i = 0; i < height; ++i) {
            std::vector<int> row;
            for (int j = 0; j < width; ++j) {
                if (!input.get(tmp)) {
                    return;
                }
                row.push_back(tmp - '0');
            }
            img.push_back(row);
        }
        image.push_back(img);
    }
}

int main()
{
    std::ifstream input{"day8.in"};
    std::ofstream output{"day8-1.out"};

    const int width = 25;
    const int height = 6;

    std::vector<std::vector<std::vector<int>>> image;

    handleInput(input, image, width, height);

    int no_zeroes = INT32_MAX;

    int tmp = 0;
    int least_zeroes_ind = 0;

    for (int layer = 0; layer < image.size(); ++layer) {
        for (int row = 0; row < image[layer].size(); ++row) {
            tmp += std::count(image[layer][row].begin(),
                              image[layer][row].end(), 0);
        }
        if (tmp < no_zeroes) {
            no_zeroes = tmp;
            least_zeroes_ind = layer;
        }
        tmp = 0;
    }

    int num_one_dig = 0;
    int num_two_dig = 0;

    for (int row = 0; row < image[least_zeroes_ind].size(); ++row) {
        num_one_dig += std::count(image[least_zeroes_ind][row].begin(),
                                  image[least_zeroes_ind][row].end(), 1);
        num_two_dig += std::count(image[least_zeroes_ind][row].begin(),
                                  image[least_zeroes_ind][row].end(), 2);
    }

    output << num_one_dig * num_two_dig;
}
