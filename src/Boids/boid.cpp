#include "boid.hpp"

glm::vec3 get_random_position()
{
    return glm::vec3{p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f)};
}

Boid::Boid(p6::Color color, glm::vec3 position, glm::vec3 velocity, float size)
    : _color(color)
    , _position(position)
    , _velocity(velocity)
    , _size(size)
{
}

Boid::Boid()
    : _color(p6::Color{0.0f, 1.0f, 1.0f})
    , _position(glm::vec3{get_random_position()})
    , _velocity(glm::vec3{0, 0, 0})
    , _size(0.01f)
{}

void Boid::update(float delta_time)
{
    _position += _velocity * delta_time;
}

glm::vec3 Boid::get_position() const
{
    return _position;
}

glm::vec3 Boid::get_velocity() const
{
    return _velocity;
}

void Boid::set_velocity(glm::vec3 newVelocity)
{
    _velocity = newVelocity;
}
