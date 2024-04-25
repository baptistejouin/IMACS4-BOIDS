#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "boidsParams.hpp"

float map_to_max_speed(float value, float max_speed);
void  apply_bounds_force(Boid& boid, const BoidsParams& params);
void  prevent_bounds_exit(Boid& boid, const BoidsParams& params);

class Boids {
public:
    explicit Boids(int nbBoids = 300);

    void update(float delta_time);
    void reset();
    void gui();

    const std::vector<Boid>& get_boids() const { return _boids; }
    const Bounds&            get_bounds() const { return _params.bounds; }

private:
    std::vector<Boid> _boids;
    BoidsParams       _params;
};