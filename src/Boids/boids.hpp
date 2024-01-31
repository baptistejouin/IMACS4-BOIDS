#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boid/boid.hpp"
#include "glm/glm.hpp"

namespace Project {

class Boids {
public:
    std::vector<Boid> boids;

    Boids(int nbBoids);

    // destructor
    ~Boids();

    // methods
    void update();
    void draw(p6::Context& ctx) const;
    void addBoid();
    void removeBoid();
};
} // namespace Project