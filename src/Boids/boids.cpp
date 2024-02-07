#include "Boids/boids.hpp"

// constructor
Boids::Boids(int nbBoids)
    : boids(nbBoids)
{
}

void Boids::update(float delta_time)
{
    for (auto& boid : boids)
    {
        boid.update(delta_time);
    }
}

void Boids::draw(p6::Context& ctx) const
{
    for (auto const& boid : boids)
    {
        boid.draw(ctx);
    }
}