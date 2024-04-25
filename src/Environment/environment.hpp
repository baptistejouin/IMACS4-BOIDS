#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera/camera.hpp"

struct Element {
    glm::vec3 position{0.f};
    glm::vec3 scale{1.f};
    glm::vec3 rotation{0.f};
};

struct Light {
    int       index;
    glm::vec3 position{0.f};
    glm::vec3 intensity{1.f};
};

struct EnvironmentParams {
    p6::Color            background_color;
    Element              terrain;
    std::vector<Light>   point_light;
    std::vector<Element> flowers;

    EnvironmentParams();
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void update_fixed_light(Camera& camera);
    void draw(p6::Context& ctx) const;
    void gui();

    const Element&              get_terrain() const { return _params.terrain; }
    const std::vector<Light>&   get_points_light() const { return _params.point_light; }
    const std::vector<Element>& get_flowers() const { return _params.flowers; }

private:
    EnvironmentParams _params;
};
