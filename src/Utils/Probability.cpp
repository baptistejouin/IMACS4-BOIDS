#include "Utils/Probability.hpp"

long double rand_0_1()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<long double>{0.000000000, 1.000000000};

    return distrib(gen);
}

glm::vec3 Probability::math_law_01()
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
