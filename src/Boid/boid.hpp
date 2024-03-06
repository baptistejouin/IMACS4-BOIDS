#pragma once

#include <glm/glm.hpp>
#include "p6/p6.h"

class Boid {
public:
    Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity, float size);
    Boid();

    void update(float delta_time);
    void draw(p6::Context& ctx) const;

    void set_position(glm::vec2 position) { this->position = position; }

private:
    p6::Color color;
    glm::vec2 position;
    glm::vec2 velocity;
    float     size;
};
