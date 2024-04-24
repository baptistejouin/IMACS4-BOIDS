#pragma once

#include <imgui.h>
#include <p6/p6.h>

struct Terrain {
    glm::vec3 position{0.f, -1.75f, 0.f};
    glm::vec3 scale{0.3f};
};

struct EnvironmentParams {
    p6::Color background_color{0.7f, 0.7f, 0.7f};
    Terrain   terrain;
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void draw(p6::Context& ctx) const;
    void gui();

    const Terrain& get_terrain() const { return _params.terrain; }

private:
    EnvironmentParams _params;
};
