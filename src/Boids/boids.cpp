#include "boids.hpp"
#include "glm/fwd.hpp"

Boids::Boids(int nbBoids)
    : _boids(nbBoids)
{
}

float mapToInfinity(float value)
{
    value = std::clamp(value, 0.0f, 1.0f);
    return -std::log(1.0f - value) * .05f;
}

void apply_bounds_force(Boid& boid, const BoidsParams& params)
{
    if (boid.get_position().x < params.bounds.x[0] + params.bounds_force_range)
    {
        float approach_normalised = (params.bounds.x[0] - boid.get_position().x) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(mapToInfinity(approach_normalised), 0, 0));
    }
    if (boid.get_position().x > params.bounds.x[1] - params.bounds_force_range)
    {
        float approach_normalised = (boid.get_position().x - params.bounds.x[1]) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(-mapToInfinity(approach_normalised), 0, 0));
    }
    if (boid.get_position().y < params.bounds.y[0] + params.bounds_force_range)
    {
        float approach_normalised = (params.bounds.y[0] - boid.get_position().y) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(0, mapToInfinity(approach_normalised), 0));
    }
    if (boid.get_position().y > params.bounds.y[1] - params.bounds_force_range)
    {
        float approach_normalised = (boid.get_position().y - params.bounds.y[1]) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(0, -mapToInfinity(approach_normalised), 0));
    }
    if (boid.get_position().z < params.bounds.z[0] + params.bounds_force_range)
    {
        float approach_normalised = (params.bounds.z[0] - boid.get_position().z) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(0, 0, mapToInfinity(approach_normalised)));
    }
    if (boid.get_position().z > params.bounds.z[1] - params.bounds_force_range)
    {
        float approach_normalised = (boid.get_position().z - params.bounds.z[1]) / params.bounds_force_range;
        boid.set_velocity(boid.get_velocity() + glm::vec3(0, 0, -mapToInfinity(approach_normalised)));
    }
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
        boid.set_position({0.f, 0.f, 0.f});
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