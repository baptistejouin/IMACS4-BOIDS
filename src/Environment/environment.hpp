#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <glm/glm.hpp>
#include <vector>

struct Element {
    glm::vec3 position;
    glm::vec3 scale;
};

struct EnvironmentParams {
    p6::Color            background_color;
    Element              terrain;
    std::vector<Element> point_light;

    EnvironmentParams()
        : background_color{0.7f, 0.7f, 0.7f}
        , terrain{{0.f, -1.75f, 0.f}, glm::vec3{0.3f}}
        , point_light{{{0.f, -.6f, 0.f}, glm::vec3{0.1f}}, {{1.f, .6f, 0.f}, glm::vec3{0.1f}}}
    {
    }
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void draw(p6::Context& ctx) const;
    void gui();

    const Element&              get_terrain() const { return _params.terrain; }
    const std::vector<Element>& get_point_light() const { return _params.point_light; }

private:
    EnvironmentParams _params;
};
