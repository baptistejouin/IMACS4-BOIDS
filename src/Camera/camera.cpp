#include "Camera/camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

Camera::Camera() noexcept
    : _distance(.3f), _position({0.0f, 0.0f, 0.0f}), _phi(glm::pi<float>()), _theta(0.0f), _front({0.0f, 0.0f, 0.0f}), _up({0.0f, 0.0f, 0.0f}), _left({0.0f, 0.0f, 0.0f})
{
    compute_direction_vectors();
}

void Camera::move_front(float t)
{
    _position += t * _front;
}

void Camera::move_left(float t)
{
    _position += t * _left;
}

void Camera::move_up(float t)
{
    _position += t * _up;
}

void Camera::compute_direction_vectors()
{
    _front = glm::vec3(
        std::cos(_theta) * std::sin(_phi),
        std::sin(_theta),
        std::cos(_theta) * std::cos(_phi)
    );
    _left = glm::vec3(
        sin(_phi + glm::half_pi<double>()),
        0,
        cos(_phi + glm::half_pi<double>())
    );
    _up = glm::cross(_front, _left);
}

void Camera::rotate_left(float degrees)
{
    _phi += glm::radians(degrees);
    compute_direction_vectors();
}

void Camera::rotate_up(float degrees)
{
    _theta += glm::radians(degrees);
    compute_direction_vectors();
}

glm::mat4 Camera::get_view_matrix() const
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, {0.0f, 0.0f, -_distance});
    view = glm::lookAt(_position - (_front * _distance), _position + _front, _up);

    return view;
}