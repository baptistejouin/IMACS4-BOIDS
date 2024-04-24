#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <glm/glm.hpp>
#include <vector>

struct Element {
    glm::vec3 position;
    glm::vec3 scale;
};

struct Light {
    int       index;
    glm::vec3 position;
    glm::vec3 intensity;
};

struct Terrain {
    glm::vec3 position{0.f, -1.75f, 0.f};
    glm::vec3 scale{0.3f};
};

struct Terrain {
    glm::vec3 position{0.f, -1.75f, 0.f};
    glm::vec3 scale{0.3f};
};

struct EnvironmentParams {
    p6::Color          background_color;
    Element            terrain;
    std::vector<Light> point_light;

    EnvironmentParams();
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void draw(p6::Context& ctx) const;
    void gui();

    const Element&            get_terrain() const { return _params.terrain; }
    const std::vector<Light>& get_points_light() const { return _params.point_light; }

private:
    EnvironmentParams _params;
};
