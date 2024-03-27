#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

class TrackballCamera {
public:
    TrackballCamera() noexcept;
    void      moveFront(float delta);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    void      drag(float x, float y, float factor);
    glm::mat4 getViewMatrix() const;

private:
    float _distance;
    float _angleX;
    float _angleY;
};