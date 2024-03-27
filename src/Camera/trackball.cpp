#include "Camera/trackball.hpp"

TrackballCamera::TrackballCamera() noexcept
    : _distance(10.0f)
    , _angleX(0.0f)
    , _angleY(0.0f)
{
}

void TrackballCamera::moveFront(float delta)
{
    _distance += delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    _angleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    _angleX += degrees;
}

void TrackballCamera::drag(float y, float x, float factor)
{
    _angleY += x * factor;
    _angleX += y * factor;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -_distance));
    view = glm::rotate(view, glm::radians(_angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(_angleY), glm::vec3(0.0f, 1.0f, 0.0f));

    return view;
}
