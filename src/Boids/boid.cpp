#include "boid.hpp"

glm::vec3 get_random_position()
{
    return glm::vec3{p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f)};
}

Boid::Boid(glm::vec3 position, glm::vec3 velocity, float size)
    : _position(position)
    , _velocity(velocity)
    , _size(size)
{
}

Boid::Boid()
    : _position(glm::vec3{get_random_position()})
    , _velocity(glm::vec3{0, 0, 0})
    , _size(0.03f)
{}

void Boid::update(float delta_time)
{
    _position += _velocity * delta_time;
}

void Boid::set_velocity(glm::vec3 new_velocity)
{
    _velocity = new_velocity;
}

std::pair<float, glm::vec3> Boid::get_look_at_angle_and_axis() const
{
    glm::vec3 up                  = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 normalized_velocity = glm::normalize(_velocity);

    float     angle = glm::acos(glm::dot(up, normalized_velocity));
    glm::vec3 axis  = glm::cross(up, normalized_velocity);

    return std::make_pair(angle, axis);
}