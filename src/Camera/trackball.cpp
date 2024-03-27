#include "Camera/trackball.hpp"

TrackballCamera::TrackballCamera() noexcept
    : _distance(10.0f)
    , _angleX(0.0f)
    , _angleY(0.0f)
{
}

void TrackballCamera::move_front(float delta)
{
    _distance += delta;
}

void TrackballCamera::rotate_left(float degrees)
{
    _angleY += degrees;
}

void TrackballCamera::rotate_up(float degrees)
{
    _angleX += degrees;
}

void TrackballCamera::drag(float deltaY, float deltaX, float factor)
{
    _angleY += deltaX * factor;
    _angleX += deltaY * factor;
}

glm::mat4 TrackballCamera::get_view_matrix() const
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -_distance));
    view = glm::rotate(view, glm::radians(_angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(_angleY), glm::vec3(0.0f, 1.0f, 0.0f));

    return view;
}
