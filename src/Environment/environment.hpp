#pragma once

#include <imgui.h>
#include "glm/glm.hpp"
#include "p6/p6.h"

struct EnvironmentParams {
    float     radius{0.8f};
    float     stroke_weight{0.005f};
    glm::vec2 center{0.f, 0.f};
    p6::Color background_color{0.7f, 0.7f, 0.7f};
    p6::Color fill_color{1.f, 1.f, 1.f};
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void draw(p6::Context& ctx) const;
    void gui();

private:
    EnvironmentParams p;
};
