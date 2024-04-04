#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

class TrackballCamera {
public:
    TrackballCamera() noexcept;
    void      move_front(float delta);
    void      rotate_left(float degrees);
    void      rotate_up(float degrees);
    void      drag(float deltaY, float deltaX, float factor);
    glm::mat4 get_view_matrix() const;

private:
    float _distance;
    float _angleX;
    float _angleY;
};