#include "Utils/Probability.hpp"

long double rand_0_1()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<long double>{0.000000000, 1.000000000};

    return distrib(gen);
}

glm::vec3 Probability::get_random_velocity()
{
    return glm::vec3{p6::random::number(-.1f, .1f), p6::random::number(-.1f, .1f), p6::random::number(-.1f, .1f)};
}

glm::vec3 Probability::get_random_position()
{
    // TODO(eliott)
    return {rand_0_1() * 1.6f - 0.8f, rand_0_1() * 1.6f - 0.8f, rand_0_1() * 1.6f - 0.8f};
};

void Probability::math_law_02()
{
}

void Probability::math_law_03()
{
}

void Probability::math_law_04()
{
}

void Probability::math_law_05()
{
}
