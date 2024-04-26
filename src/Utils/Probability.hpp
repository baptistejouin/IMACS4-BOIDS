#pragma once

#include <p6/p6.h>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <random>

class Probability {
public:
    static glm::vec3   get_random_position();
    static glm::vec3   get_random_velocity(float min, float max);
    static std::string get_random_boid_texture();
    static int         choose_flower_model(int& current_flower);
    static float       get_boid_scale();
};