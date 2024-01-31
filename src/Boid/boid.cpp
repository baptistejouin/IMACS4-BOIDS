#include "Boid/boid.hpp"

// constructor
Project::Boid::Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity)
    : color(color)
    , position(position)
    , velocity(velocity)
{
}

Project::Boid::Boid()
{
    glm::vec2 random_velocity{p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)};
    random_velocity = glm::normalize(velocity);

    color    = p6::Color{0.0f, 1.0f, 1.0f};
    position = glm::vec2{0.0f, 0.0f};
    velocity = random_velocity;
}

// destructor
Project::Boid::~Boid()
{
}

void Project::Boid::update()
{
    position += velocity * 0.001f;
}

void Project::Boid::draw(p6::Context& ctx) const
{
    ctx.fill = color;
    ctx.circle(p6::Center(position), p6::Radius(0.01f));
}