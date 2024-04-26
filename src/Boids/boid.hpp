#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <string>
#include "Utils/Probability.hpp"
#include "glm/fwd.hpp"

class Boid {
public:
    Boid(glm::vec3 position, glm::vec3 velocity, float size);
    Boid();

    void update(float delta_time);

    glm::vec3                   get_position() const { return _position; }
    glm::vec3                   get_velocity() const { return _velocity; }
    float                       get_size() const { return _size; }
    std::pair<float, glm::vec3> get_look_at_angle_and_axis() const;

    void        set_velocity(glm::vec3 new_velocity);
    void        set_position(glm::vec3 position) { _position = position; }
    std::string get_texture_name() const { return _texture_name; }

private:
    glm::vec3   _position;
    glm::vec3   _velocity;
    float       _size;
    std::string _texture_name;
};