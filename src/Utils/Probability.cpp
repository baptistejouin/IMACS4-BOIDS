#include "Utils/Probability.hpp"
#include "glm/common.hpp"

/*
 * Loi utiles pour la simulation de boids
 */

long double rand_0_1()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<long double>{0.000000000, 1.000000000};

    return distrib(gen);
}

bool bernoulli(float p)
{
    return rand_0_1() < p;
}
double normale(double min, double max) // méthode de box-muller
{
    double mu    = (min + max) / 2;
    double sigma = (max - min) / 3;
    double X     = sqrt(-2 * log(rand_0_1())) * cos(2 * M_PI * rand_0_1());
    return glm::clamp(mu + sigma * X, min, max);
}

double uniform(const double min, const double max)
{
    return (max - min) * rand_0_1() + min;
}

glm::mat4 mat{{0.1, 0.3, 0.3, 0.3}, {0.3, 0.1, 0.3, 0.3}, {0.3, 0.3, 0.1, 0.3}, {0.3, 0.3, 0.3, 0.1}};

int markov_next(const glm::vec4& matrix)
{
    float r = rand_0_1();

    if (r < matrix[0])
    {
        return 0;
    }
    if (r < matrix[0] + matrix[1])
    {
        return 1;
    }
    if (r < matrix[0] + matrix[1] + matrix[2])
    {
        return 2;
    }
    return 3;
}

double exponential(double min, double max, double lambda)
{
    // génération d'un float selon loi exponentielle avec la méthode de l'inverse de la fonction de répartition
    double u1 = rand_0_1();
    double u2 = -log(1 - u1) / lambda;
    return min + (max - min) * u2;
}

/*
 * Fonctions qui utilisent les lois de probabilités pour affecter le programme
 */

std::string Probability::get_random_boid_texture() // utilisation de la loi de Bernoulli
{
    if (bernoulli(1.f / 3.f))
    {
        return "papier.jpg";
    }

    if (bernoulli(1.f / 3.f))
    {
        return "papier-bleu.jpg";
    }

    return "papier-jaune.jpg";
}

glm::vec3 Probability::get_random_velocity(float min, float max) // utilisation de la loi uniforme
{
    return glm::vec3{uniform(min, max), uniform(min, max), uniform(min, max)};
}

glm::vec3 Probability::get_random_position() // utilisation de la loi normale
{
    return {normale(-0.8, 0.8), normale(-0.8, 0.8), normale(-0.8, 0.8)};
}

int Probability::choose_flower_model(int& current_flower_model_type) // utilisation de la chaîne de Markov
{
    /**
     * choose a flower model with the markov chain :
     * flower_red = 0
     * flower_blue = 1
     * flower_yellow = 2
     * flower_white = 3
     */

    // Matrice de passage pour chaîne de Markov

    switch (current_flower_model_type)
    {
    case 0:
        return markov_next(mat[0]);
        break;
    case 1:
        return markov_next(mat[1]);
        break;
    case 2:
        return markov_next(mat[2]);
        break;
    case 3:
        return markov_next(mat[3]);
        break;
    }
}

float Probability::get_boid_scale()
{
    // original size 0.03
    // lambda = 2.0f : la distribution exponentielle sera plus étroite, avec une concentration plus forte autour de min
    return exponential(0.01, 0.035, 2.);
}