#pragma once

#include <glm/glm.hpp>

struct Bounds {
    glm::vec2 x;
    glm::vec2 y;
    glm::vec2 z;
};

struct BoidsParams {
    float  visual_range{.15f};
    float  protected_range{.065f};
    float  centering_factor{.001f};
    float  avoid_factor{.9f};
    float  matching_factor{.02f};
    float  max_speed{.8f};
    float  min_speed{.5f};
    Bounds bounds{{-1.45f, 1.45f}, {-1.0f, 1.45f}, {-1.45f, 1.45f}};
    float  bounds_force_range{0.3f};
};