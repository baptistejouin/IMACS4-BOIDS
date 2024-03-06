#include "boid.hpp"

glm::vec2 get_random_position()
{
    return glm::vec2{p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f)}; // Square radius
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
    : color(p6::Color{0.0f, 1.0f, 1.0f})
    , position(glm::vec2{get_random_position()})
    , velocity(glm::vec2{0, 0})
    , size(0.01f)
{}

void Boid::update(float delta_time, BoidsParams const& params)
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

glm::vec2 Boid::getPosition() const
{
    return position;
}

glm::vec2 Boid::getVelocity() const
{
    return velocity;
}

void Boid::setVelocity(glm::vec2 newVelocity)
{
    velocity = newVelocity;
}