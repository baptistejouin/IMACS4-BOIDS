#include "Boids/boids.hpp"

// constructor
Project::Boids::Boids(int nbBoids)
{
    for (int i = 0; i < nbBoids; i++)
    {
        boids.emplace_back(Boid{.size = .01f, .color = {1.f, 1.f, 1.f}, .position = {0.f, 0.f}});
    }
}