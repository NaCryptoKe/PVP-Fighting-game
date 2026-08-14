#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <glm/vec3.hpp>

struct Face
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

class Model
{
public:
    std::vector<glm::vec3> positions;
    std::vector<Face> faces;
};

class ModelLoader
{
public:
    Model load(const char* filename);
};

#endif // MODELLOADER_H