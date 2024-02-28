#include "Boid/boid.hpp"

glm::vec2 get_random_velocity()
{
    return glm::vec2{p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)};
}

// constructor
Boid::Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity, float size)
    : color(color)
    , position(position)
    , velocity(velocity)
    , size(size)
{
}

Boid::Boid()
    : color(p6::Color{1.0f, 0.0f, 0.0f})
    , position(glm::vec2{0.0f, 0.0f})
    , velocity(glm::vec2{get_random_velocity()})
    , size(0.01f)
{}

void Boid::update(float delta_time)
{
    position += velocity * delta_time;
}

void Boid::draw(p6::Context& ctx) const
{
    ctx.fill          = color;
    ctx.stroke_weight = 0.0f;

    // calculate the angle of the velocity vector
    float angle = std::atan2(velocity.y, velocity.x);

    // draw the boid
    p6::Point2D p1 = position + glm::vec2{std::cos(angle), std::sin(angle)} * size * 2.0f;
    p6::Point2D p2 = position + glm::vec2{std::cos(angle + 2.0f), std::sin(angle + 2.0f)} * size;
    p6::Point2D p3 = position + glm::vec2{std::cos(angle - 2.0f), std::sin(angle - 2.0f)} * size;

    ctx.triangle(p1, p2, p3);
}