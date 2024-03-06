#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "boidsParams.hpp"

class Boids {
public:
    Boids(int nbBoids);

    // methods
    void update(float delta_time);
    void draw(p6::Context& ctx) const;
    void addBoid();
    void removeBoid();

    BoidsParams params;

private:
    std::vector<Boid> boids;
};