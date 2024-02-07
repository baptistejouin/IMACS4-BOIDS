#include "boids.hpp"

// constructor
Boids::Boids(int nbBoids)
    : boids(nbBoids)
    , params{.turnFactor = 0.2f, .visualRange = 40.0f, .protectedRange = 8.0f, .centeringFactor = 0.0005f, .avoidFactor = 0.05f, .matchingFactor = 0.005f, .maxSpeed = 6.0f, .minSpeed = 2.0f}
{
}

void Boids::update(float delta_time)
{
    for (auto& boid : boids)
    {
        boid.update(delta_time, params);
    }
}

void Boids::draw(p6::Context& ctx) const
{
    for (auto const& boid : boids)
    {
        boid.draw(ctx);
    }
}