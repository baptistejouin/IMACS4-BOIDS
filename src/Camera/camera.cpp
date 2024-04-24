#include "Camera/camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

Camera::Camera() noexcept
    : _distance(.3f), _position({0.0f, 0.0f, 0.0f}), _phi(glm::pi<float>()), _theta(0.0f), _front({0.0f, 0.0f, 0.0f}), _up({0.0f, 0.0f, 0.0f}), _left({0.0f, 0.0f, 0.0f})
{
    compute_direction_vectors();
}

bool Camera::cube_collision(glm::vec3 next_position, float t, Bounds bounds) const
{
    if (next_position.x <= bounds.x[0] + t * 2)
    {
        return true;
    }

    if (next_position.x >= bounds.x[1] - t * 2)
    {
        return true;
    }

    if (next_position.y <= bounds.y[0] + t * 2)
    {
        return true;
    }

    if (next_position.y >= bounds.y[1] - t * 2)
    {
        return true;
    }

    if (next_position.z <= bounds.z[0] + t * 2)
    {
        return true;
    }

    if (next_position.z >= bounds.z[1] - t * 2)
    {
        return true;
    }
    return false;
}

void Camera::move_front(float t, Bounds bounds)
{
    if (!cube_collision(_position + t * _front, t, bounds))
    {
        _position += t * _front;
    }
}

void Camera::move_left(float t, Bounds bounds)
{
    if (!cube_collision(_position + t * _left, t, bounds))
    {
        _position += t * _left;
    }
}

void Camera::move_up(float t, Bounds bounds)
{
    if (!cube_collision(_position + t * glm::vec3(0.0f, 1.0f, 0.0f), t, bounds))
    {
        _position += t * glm::vec3(0.0f, 1.0f, 0.0f);
    }
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

void Camera::zoom(float delta, float factor)
{
    if (_distance + delta * factor < 5.0f && _distance + delta * factor > 0.15f)
        _distance += delta * factor;
}

glm::mat4 Camera::get_view_matrix() const
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, {0.0f, 0.0f, -_distance});
    view = glm::lookAt(_position - (_front * _distance), _position + _front, _up);

    return view;
}