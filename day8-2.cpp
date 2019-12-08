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

int getPixelValue(std::vector<std::vector<std::vector<int>>>& image, int x,
                  int y)
{
    // find the first non-transparent pixel
    for (int layer = 0; layer < image.size(); ++layer) {
        if (image[layer][y][x] != 2) {
            return image[layer][y][x];
        }
    }
    return -1;
}

int main()
{
    std::ifstream input{"day8.in"};
    std::ofstream output{"day8-2.out"};

    const int width = 25;
    const int height = 6;

    std::vector<std::vector<std::vector<int>>> image;

    handleInput(input, image, width, height);

    for (int y = 0; y < image[0].size(); ++y) {
        for (int x = 0; x < image[0][y].size(); ++x) {
            output << getPixelValue(image, x, y);
        }
        output << '\n';
    }

    // now look at the output and read the message
}
