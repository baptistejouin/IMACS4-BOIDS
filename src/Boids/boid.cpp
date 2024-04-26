#include "boid.hpp"
#include "Utils/Probability.hpp"
#include "glm/fwd.hpp"

Boid::Boid(glm::vec3 position, glm::vec3 velocity, float size)
    : _position(position)
    , _velocity(velocity)
    , _size(size)
    , _texture_name(Probability::get_random_boid_texture())
{
}

Boid::Boid()
    : _position(glm::vec3{Probability::get_random_position()})
    , _velocity(glm::vec3{Probability::get_random_velocity()})
    , _size(0.03f)
    , _texture_name(Probability::get_random_boid_texture())
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