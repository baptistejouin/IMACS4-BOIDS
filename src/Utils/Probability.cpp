#include "Utils/Probability.hpp"
#include "glm/common.hpp"

long double rand_0_1()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<long double>{0.000000000, 1.000000000};

    return distrib(gen);
}

bool Probability::bernoulli(float p)
{
    return rand_0_1() < p;
}

std::string Probability::get_random_boid_texture()
{
    if (bernoulli(1. / 3.f))
    {
        return "papier.jpg";
    }
    else if (bernoulli(1. / 3.f))
    {
        return "papier-bleu.jpg";
    }
    else
    {
        return "papier-jaune.jpg";
    }
}

double Probability::normale(double min, double max) // méthode de box-muller
{
    double mu    = (min + max) / 2;
    double sigma = (max - min) / 3;
    double X     = sqrt(-2 * log(rand_0_1())) * cos(2 * M_PI * rand_0_1());
    return glm::clamp(mu + sigma * X, min, max);
}

glm::vec3 Probability::get_random_velocity()
{
    return glm::vec3{p6::random::number(-.1f, .1f), p6::random::number(-.1f, .1f), p6::random::number(-.1f, .1f)};
}

glm::vec3 Probability::get_random_position()
{
    return {normale(-0.8, 0.8), normale(-0.8, 0.8), normale(-0.8, 0.8)};
}

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
