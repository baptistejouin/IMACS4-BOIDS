#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "boidsParams.hpp"

class Boids {
public:
    Boids(int nbBoids = 250);

    // methods
    void update(float delta_time);
    void draw(p6::Context& ctx) const;
    void addBoid();
    void removeBoid();
    void reset();
    void gui();

private:
    std::vector<Boid> boids;
    BoidsParams       params;
};