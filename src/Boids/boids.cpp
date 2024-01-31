#include "Boids/boids.hpp"

// constructor
Project::Boids::Boids(int nbBoids)
{
    for (int i = 0; i < nbBoids; i++)
    {
        Boid boid;

        boids.push_back(boid);
    }
}

// destructor
Project::Boids::~Boids()
{
}

void Project::Boids::update()
{
    for (auto& boid : boids)
    {
        boid.update();
    }
}

void Project::Boids::draw(p6::Context& ctx) const
{
    for (auto& boid : boids)
    {
        boid.draw(ctx);
    }
}