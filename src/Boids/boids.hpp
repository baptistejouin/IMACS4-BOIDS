#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "Utils/Probability.hpp"
#include "boidsParams.hpp"

float map_to_max_speed(float value, float max_speed);
void  apply_bounds_force(Boid& boid, const BoidsParams& params);
void  prevent_bounds_exit(Boid& boid, const BoidsParams& params);

class Boids {
public:
    explicit Boids(int nbBoids = 500);

    void update(float delta_time);
    void reset();
    void gui();

    const std::vector<Boid>& get_boids() const { return _boids; }
    const Bounds&            get_bounds() const { return _params.bounds; }
    void                     update_boids_params();

private:
    int               _state = 0;
    std::vector<Boid> _boids;
    BoidsParams       _params;
};