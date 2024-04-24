#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/fwd.hpp"

class Camera {
public:
    Camera() noexcept;
    void      move_front(float delta);
    void      move_left(float delta);
    void      rotate_left(float degrees);
    void      rotate_up(float degrees);
    glm::mat4 get_view_matrix() const;
    glm::vec3 get_position() const { return _position; }

private:
    float     _distance;
    glm::vec3 _position;
    float     _phi;
    float     _theta;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _left;
    void      compute_direction_vectors();
};