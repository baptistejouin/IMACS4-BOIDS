#include "boid.hpp"

glm::vec2 get_random_position()
{
    return glm::vec2{p6::random::number(-.8f, .8f), p6::random::number(-.8f, .8f)};
}

// constructor
Boid::Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity, float size)
    : color(color)
    , position(position)
    , velocity(velocity)
    , size(size)
{
}

Boid::Boid()
    : color(p6::Color{0.0f, 1.0f, 1.0f})
    , position(glm::vec2{get_random_position()})
    , velocity(glm::vec2{0, 0})
    , size(0.01f)
{}

void Boid::update(float delta_time)
{
    position += velocity * delta_time;
}

void Boid::draw(const VAO& vao, const GLint& uMVPMatrixLocation, const GLint& uMVMatrixLocation, const GLint& uNormalMatrixLocation, glm::mat4& ProjMatrix, const std::vector<ShapeVertex>& vertices, TrackballCamera& camera) const
{
    glm::mat4 MVMatrix = glm::translate(
        glm::mat4(1.f),
        glm::vec3(position.x, position.y, -20.f)
    );
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 ViewMatrix   = camera.getViewMatrix();
    glm::mat4 MVPMatrix    = ProjMatrix * ViewMatrix * MVMatrix;

    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
    glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    vao.bind();
    // think about texture here (binding)

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    vao.unbind();
}

glm::vec2 Boid::getPosition() const
{
    return position;
}

glm::vec2 Boid::getVelocity() const
{
    return velocity;
}

void Boid::setVelocity(glm::vec2 newVelocity)
{
    velocity = newVelocity;
}
