#include "boids.hpp"
#include <cstdio>
#include "Utils/Probability.hpp"
#include "glm/fwd.hpp"

Boids::Boids(int nbBoids)
    : _boids(nbBoids)
{
}

float map_to_max_speed(float value, float max_speed)
{
    value = std::clamp(value, 0.0f, 1.0f);
    value *= value;
    return (1 - value) * max_speed * 0.1;
}

void apply_bounds_force(Boid& boid, const BoidsParams& params)
{
    glm::vec3 position = boid.get_position();
    glm::vec3 velocity = boid.get_velocity();

    if (position.x <= params.bounds.x[0] + params.bounds_force_range)
    {
        float approach            = position.x - params.bounds.x[0];
        float approach_normalised = approach / params.bounds_force_range;
        velocity += glm::vec3(map_to_max_speed(approach_normalised, params.max_speed), 0, 0);
    }

    if (position.x >= params.bounds.x[1] - params.bounds_force_range)
    {
        float approach            = params.bounds.x[1] - position.x;
        float approach_normalised = approach / params.bounds_force_range;
        velocity -= glm::vec3(map_to_max_speed(approach_normalised, params.max_speed), 0, 0);
    }

    if (position.y <= params.bounds.y[0] + params.bounds_force_range)
    {
        float approach            = position.y - params.bounds.y[0];
        float approach_normalised = approach / params.bounds_force_range;
        velocity += glm::vec3(0, map_to_max_speed(approach_normalised, params.max_speed), 0);
    }

    if (position.y >= params.bounds.y[1] - params.bounds_force_range)
    {
        float approach            = params.bounds.y[1] - position.y;
        float approach_normalised = approach / params.bounds_force_range;
        velocity -= glm::vec3(0, map_to_max_speed(approach_normalised, params.max_speed), 0);
    }

    if (position.z <= params.bounds.z[0] + params.bounds_force_range)
    {
        float approach            = position.z - params.bounds.z[0];
        float approach_normalised = approach / params.bounds_force_range;
        velocity += glm::vec3(0, 0, map_to_max_speed(approach_normalised, params.max_speed));
    }

    if (position.z >= params.bounds.z[1] - params.bounds_force_range)
    {
        float approach            = params.bounds.z[1] - position.z;
        float approach_normalised = approach / params.bounds_force_range;
        velocity -= glm::vec3(0, 0, map_to_max_speed(approach_normalised, params.max_speed));
    }

    boid.set_velocity(velocity);
}

void prevent_bounds_exit(Boid& boid, const BoidsParams& params)
{
    glm::vec3 position = boid.get_position();

    if (position.x < params.bounds.x[0])
    {
        position.x = params.bounds.x[0];
    }
    if (position.x > params.bounds.x[1])
    {
        position.x = params.bounds.x[1];
    }
    if (position.y < params.bounds.y[0])
    {
        position.y = params.bounds.y[0];
    }
    if (position.y > params.bounds.y[1])
    {
        position.y = params.bounds.y[1];
    }
    if (position.z < params.bounds.z[0])
    {
        position.z = params.bounds.z[0];
    }
    if (position.z > params.bounds.z[1])
    {
        position.z = params.bounds.z[1];
    }

    boid.set_position(position);
}

void Boids::update(float delta_time)
{
    for (auto& boid : _boids)
    {
        // Zero all accumulator variables (can't do this in one line in C)
        glm::vec3 posAvg           = {0, 0, 0};
        glm::vec3 velAvg           = {0, 0, 0};
        glm::vec3 closeD           = {0, 0, 0};
        int       neighboringBoids = 0;

        // For every other boid in the flock
        for (auto const& otherBoid : _boids)
        {
            if (&boid != &otherBoid)
            {
                // Calculate the distance between the two boids
                glm::vec3 d = boid.get_position() - otherBoid.get_position();

                if (std::fabs(d.x) < _params.visual_range && std::fabs(d.y) < _params.visual_range && std::fabs(d.z) < _params.visual_range)
                {
                    float distance = glm::length(d);

                    if (distance < _params.protected_range)
                    {
                        closeD += d / distance * (_params.protected_range - distance);
                    }
                    // If the other boid is within the visual range
                    else if (distance < _params.visual_range)
                    {
                        // Add the position and velocity to the average
                        posAvg += otherBoid.get_position();
                        velAvg += otherBoid.get_velocity();

                        // Increment the number of neighboring boids
                        neighboringBoids++;
                    }
                }
            }
        }

        glm::vec3 centeringForce = {0, 0, 0};
        glm::vec3 matchingForce  = {0, 0, 0};
        glm::vec3 avoidanceForce = {0, 0, 0};

        if (neighboringBoids > 0)
        {
            // Divide accumulator variables by the number of neighboring boids
            posAvg /= neighboringBoids;
            velAvg /= neighboringBoids;

            // Calcul de la différence entre la position moyenne des voisins et la position actuelle du boid
            centeringForce = (posAvg - boid.get_position()) * _params.centering_factor;

            // Calcul de la différence entre la vitesse moyenne des voisins et la vitesse actuelle du boid
            matchingForce = (velAvg - boid.get_velocity()) * _params.matching_factor;
        }
        // Add avoidance force to the boid's velocity
        avoidanceForce = closeD * _params.avoid_factor;

        boid.set_velocity(boid.get_velocity() + avoidanceForce + centeringForce + matchingForce);

        float speed = glm::length(boid.get_velocity());

        apply_bounds_force(boid, _params);
        prevent_bounds_exit(boid, _params);

        // Normalize the boid's velocity
        boid.set_velocity(glm::normalize(boid.get_velocity()) * speed);

        // Enforce minimum and maximum speeds
        if (speed > _params.max_speed)
        {
            boid.set_velocity(glm::normalize(boid.get_velocity()) * _params.max_speed);
        }
        else if (speed < _params.min_speed)
        {
            boid.set_velocity(glm::normalize(boid.get_velocity()) * _params.min_speed);
        }

        // Update the boid's position
        boid.update(delta_time);
    }
}

void Boids::reset()
{
    for (auto& boid : _boids)
    {
        boid.set_position(Probability::get_random_position());
        boid.set_velocity(Probability::get_random_velocity(-0.1f, 0.1f));
    }
}

void Boids::update_boids_params()
{
    _state = Probability::get_random_boids_params(_state);

    BoidsParams state01{
        .visual_range{.10f},
        .protected_range{.03f},
        .centering_factor{.001f},
        .avoid_factor{.9f},
        .matching_factor{.02f},
        .max_speed{.5f},
        .min_speed{.3f},
        .bounds{{-1.45f, 1.45f}, {-1.0f, 1.45f}, {-1.45f, 1.45f}},
        .bounds_force_range{0.3f},
    };
    BoidsParams state02{
        .visual_range{.15f},
        .protected_range{.04f},
        .centering_factor{.005f},
        .avoid_factor{.9f},
        .matching_factor{.02f},
        .max_speed{.2f},
        .min_speed{.1f},
        .bounds{{-1.45f, 1.45f}, {-1.0f, 1.45f}, {-1.45f, 1.45f}},
        .bounds_force_range{0.3f},
    };
    BoidsParams state03{
        .visual_range{.25f},
        .protected_range{.15f},
        .centering_factor{.001f},
        .avoid_factor{.5f},
        .matching_factor{.05f},
        .max_speed{.5f},
        .min_speed{.3f},
        .bounds{{-1.45f, 1.45f}, {-1.0f, 1.45f}, {-1.45f, 1.45f}},
        .bounds_force_range{0.3f},
    };
    BoidsParams state04{
        .visual_range{.15f},
        .protected_range{.065f},
        .centering_factor{.01f},
        .avoid_factor{.9f},
        .matching_factor{.1f},
        .max_speed{.9f},
        .min_speed{.7f},
        .bounds{{-1.45f, 1.45f}, {-1.0f, 1.45f}, {-1.45f, 1.45f}},
        .bounds_force_range{0.3f},
    };

    // return corresponding state
    switch (_state)
    {
    case 0:
        _params = state01;
        break;
    case 1:
        _params = state02;
        break;
    case 2:
        _params = state03;
        break;
    case 3:
        _params = state04;
        break;
    }
}

void Boids::gui()
{
    ImGui::Begin("Boids");
    ImGui::Text("Number of boids: %zu", _boids.size());
    if (ImGui::Button("Reset boids"))
    {
        reset();
    }
    ImGui::SliderFloat("Visual range", &_params.visual_range, .001f, .5f);
    ImGui::SliderFloat("Protected range", &_params.protected_range, .001f, .5f);
    ImGui::SliderFloat("Centering factor", &_params.centering_factor, .001f, .5f);
    ImGui::SliderFloat("Avoid factor", &_params.avoid_factor, .001f, 1.f);
    ImGui::SliderFloat("Matching factor", &_params.matching_factor, .001f, 1.f);
    ImGui::SliderFloat("Max speed", &_params.max_speed, .001f, 1.f);
    ImGui::SliderFloat("Min speed", &_params.min_speed, .001f, 1.f);
    ImGui::SliderFloat("Bounds force range", &_params.bounds_force_range, .001f, .5f);
    ImGui::End();
}