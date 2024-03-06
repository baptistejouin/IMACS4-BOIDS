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

void Boids::reset()
{
    for (auto& boid : boids)
    {
        boid.set_position({0.f, 0.f});
    }
}

void Boids::gui()
{
    ImGui::Begin("Boids");
    ImGui::Text("Number of boids: %d", boids.size());
    if (ImGui::Button("Reset boids"))
    {
        reset();
    }
    ImGui::End();
}