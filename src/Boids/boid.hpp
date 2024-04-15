#pragma once

#include <glm/glm.hpp>
#include "Camera/trackball.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "boidsParams.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

class Boid {
public:
    Boid(p6::Color color, glm::vec3 position, glm::vec3 velocity, float size);
    Boid();

    void update(float delta_time);

    glm::vec3                   get_position() const { return _position; }
    glm::vec3                   get_velocity() const { return _velocity; }
    float                       get_size() const { return _size; }
    std::pair<float, glm::vec3> get_look_at_angle_and_axis() const;

    void set_velocity(glm::vec3 newVelocity);
    void set_position(glm::vec3 position) { _position = position; }
    void set_color(p6::Color color) { _color = color; }

private:
    p6::Color _color;
    glm::vec3 _position;
    glm::vec3 _velocity;
    float     _size;
};
