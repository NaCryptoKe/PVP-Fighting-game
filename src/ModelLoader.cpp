#include "src/ModelLoader.h"

#include <fstream>
#include <sstream>
#include <string>

Model ModelLoader::load(const char* filename)
{
    Model model;

    std::ifstream file(filename);

    if (!file.is_open())
        return model;

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string type;
        ss >> type;

        if (type == "v")
        {
            float x, y, z;

            ss >> x >> y >> z;

            model.positions.emplace_back(x, y, z);
        }
        else if (type == "f")
        {
            unsigned int a, b, c;

            ss >> a >> b >> c;

            model.faces.push_back({
                a - 1,
                b - 1,
                c - 1
            });
        }
    }

    return model;
}