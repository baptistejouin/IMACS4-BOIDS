#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "boidsParams.hpp"

float mapToInfinity(float value);
void  apply_bounds_force(Boid& boid, const BoidsParams& params);

class Boids {
public:
    explicit Boids(int nbBoids = 500);

    void update(float delta_time);
    void reset();
    void gui();

    const std::vector<Boid>& get_boids() const { return _boids; }

private:
    std::vector<Boid> _boids;
    BoidsParams       _params;
};