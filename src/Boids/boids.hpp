#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Project {

struct Boid {
    float     size;
    glm::vec3 color;
    glm::vec2 position;
};

class Boids {
public:
    std::vector<Boid> boids;

    Boids(int nbBoids);

    // destructor
    ~Boids();

    // methods
    void update();
    void draw() const;
    void addBoid();
    void removeBoid();
};
} // namespace Project