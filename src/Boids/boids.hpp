#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "boidsParams.hpp"

class Boids {
public:
    Boids(int nbBoids = 500);

    void update(float delta_time);
    void reset();
    void gui();

    std::vector<Boid> get_boids();

private:
    std::vector<Boid> _boids;
    BoidsParams       _params;
};