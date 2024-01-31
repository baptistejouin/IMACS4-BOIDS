#pragma once

#include <glm/glm.hpp>
#include "glm/glm.hpp"
#include "p6/p6.h"

namespace Project {

class Boid {
public:
    Boid(p6::Color color, glm::vec2 position, glm::vec2 velocity);
    Boid();
    ~Boid();

    void update();
    void draw(p6::Context& ctx) const;

private:
    p6::Color color;
    glm::vec2 position;
    glm::vec2 velocity;
};

} // namespace Project