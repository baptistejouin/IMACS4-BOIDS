#pragma once

#include <p6/p6.h>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <random>

class Probability {
public:
    // TODO rename to true utility name
    static glm::vec3 get_random_position();
    static glm::vec3 get_random_velocity();
    static void      math_law_02();
    static void      math_law_03();
    static void      math_law_04();
    static void      math_law_05();
};