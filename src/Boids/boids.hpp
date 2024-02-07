#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boid/boid.hpp"
#include "glm/glm.hpp"

class Boids {
public:
    Boids(int nbBoids);

    // methods
    void update(float delta_time);
    void draw(p6::Context& ctx) const;
    void addBoid();
    void removeBoid();

private:
    std::vector<Boid> boids;
};