#pragma once
#include "glm/glm.hpp"
#include "p6/p6.h"

class Environment {
public:
    Environment();

    void draw(p6::Context& ctx) const;

private:
    float     radius;
    float     stroke_weight;
    glm::vec2 center;
    p6::Color background_color;
    p6::Color fill_color;
};
