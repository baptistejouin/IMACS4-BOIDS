#include "Utils/Probability.hpp"

/*
 * Loi utiles mathématiques
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
    double X     = sqrt(-2 * log(rand_0_1())) * cos(2 * M_PI * rand_0_1()); // E = 0

    return glm::clamp(mu + sigma * X, min, max);
}

double uniform(const double min, const double max)
{
    return (max - min) * rand_0_1() + min;
}

// loi beta
double beta_distrib(double alpha, double beta)
{
    // utilisation de la méthode de l'inverse de la fonction de répartition
    // une autre solution est d'utiliser la distribution beta de la librairie random C++17

    long double u1 = rand_0_1();
    long double u2 = rand_0_1();

    long double x = pow(u1, 1.0 / alpha);
    long double y = pow(u2, 1.0 / beta);

    return x / (x + y);
}

double beta_distribution(double alpha, double beta)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::gamma_distribution<double> gamma1(alpha, 1.0), gamma2(beta, 1.0);

    double x1 = gamma1(gen);
    double x2 = gamma2(gen);

    return x1 / (x1 + x2);
}

// 0% de chance de rester sur le même état - X% de chance de passer à l'état 1 - X% de chance de passer à l'état 2 - X% de chance de passer à l'état 3
glm::mat4 mat{
    {0.0, 0.4, 0.3, 0.3},
    {0.3, 0.0, 0.3, 0.4},
    {0.4, 0.3, 0.0, 0.3},
    {0.5, 0.2, 0.3, 0.0}
};

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
    // E = 1/lambda
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
    // E =  min +max / 2
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

int Probability::get_random_boids_params(int& current)
{
    switch (current)
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
double loiBeta(float alpha, float beta)
{
    float x, y;
    do
    {
        float u = rand_0_1();
        float v = rand_0_1();

        x = std::pow(u, 1.0f / alpha);
        y = std::pow(v, 1.0f / beta);
    } while (x + y > 1.0f);

    return (2.0f * x / (x + y)) - 1.0f;
}

float Probability::get_boid_scale()
{
    // loi exponentielle
    // float x = exponential(0.01, 0.035, 2.);

    // beta reduite
    float x = beta_distribution(2.0f, 2.0f) / 20;

    return x;
}
