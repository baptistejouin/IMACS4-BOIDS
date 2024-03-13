#pragma once

#include <glm/glm.hpp>
#include "boidsParams.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
public:
    Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity, float size);
    Boid();

    void update(float delta_time, BoidsParams const& params);
    void draw(p6::Context& ctx) const;

    glm::vec2 getPosition() const;
    glm::vec2 getVelocity() const;
    void      setVelocity(glm::vec2 newVelocity);
    void      set_position(glm::vec2 position) { this->position = position; }
    void      set_color(p6::Color color) { this->color = color; }

private:
    p6::Color color;
    glm::vec2 position;
    glm::vec2 velocity;
    float     size;
};
