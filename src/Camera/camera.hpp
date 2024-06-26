#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Boids/boidsParams.hpp"
#include "glm/fwd.hpp"

class Camera {
public:
    Camera() noexcept;
    bool      cube_collision(glm::vec3 next_position, Bounds bounds) const;
    void      move_front(float t, Bounds bounds);
    void      move_left(float t, Bounds bounds);
    void      move_up(float t, Bounds bounds);
    void      rotate_left(float degrees);
    void      rotate_up(float degrees);
    void      zoom(float delta, float factor);
    glm::mat4 get_view_matrix() const;
    glm::vec3 get_position() const { return _position; }
    glm::vec3 get_front() const { return _front; }
    float     get_distance() const { return _distance; }

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