#pragma once

#include <glm/glm.hpp>
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
    void draw(const VAO& vao, const GLint& uMVPMatrixLocation, const GLint& uMVMatrixLocation, const GLint& uNormalMatrixLocation, glm::mat4& ProjMatrix, const std::vector<ShapeVertex>& vertices) const;

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    void      setVelocity(glm::vec3 newVelocity);
    void      set_position(glm::vec3 position) { this->position = position; }
    void      set_color(p6::Color color) { this->color = color; }

private:
    p6::Color color;
    glm::vec3 position;
    glm::vec3 velocity;
    float     size;
};
