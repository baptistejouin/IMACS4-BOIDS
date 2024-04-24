#include "boids.hpp"
#include "glm/fwd.hpp"

Boids::Boids(int nbBoids)
    : _boids(nbBoids)
{
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

        // If the boid is near an edge, make it turn by turnFactor
        if (boid.get_position().x < _params.bounds.x[0]) // Square radius
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(_params.turn_factor, 0, 0));
        }
        if (boid.get_position().x > _params.bounds.x[1])
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(-_params.turn_factor, 0, 0));
        }
        if (boid.get_position().y < _params.bounds.y[0])
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(0, _params.turn_factor, 0));
        }
        if (boid.get_position().y > _params.bounds.y[1])
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(0, -_params.turn_factor, 0));
        }
        if (boid.get_position().z < _params.bounds.z[0])
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(0, 0, _params.turn_factor));
        }
        if (boid.get_position().z > _params.bounds.z[1])
        {
            boid.set_velocity(boid.get_velocity() + glm::vec3(0, 0, -_params.turn_factor));
        }

        float speed = glm::length(boid.get_velocity());

        // Enforce minimum and maximum speeds
        if (speed > _params.max_speed)
        {
            boid.set_velocity(boid.get_velocity() * (_params.max_speed / speed));
        }
        else if (speed < _params.min_speed)
        {
            boid.set_velocity(boid.get_velocity() * (_params.min_speed / speed));
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
    ImGui::SliderFloat("Turn factor", &_params.turn_factor, .001f, .5f);
    ImGui::SliderFloat("Visual range", &_params.visual_range, .001f, .5f);
    ImGui::SliderFloat("Protected range", &_params.protected_range, .001f, .5f);
    ImGui::SliderFloat("Centering factor", &_params.centering_factor, .001f, .5f);
    ImGui::SliderFloat("Avoid factor", &_params.avoid_factor, .001f, 1.f);
    ImGui::SliderFloat("Matching factor", &_params.matching_factor, .001f, 1.f);
    ImGui::SliderFloat("Max speed", &_params.max_speed, .001f, 1.f);
    ImGui::SliderFloat("Min speed", &_params.min_speed, .001f, 1.f);
    ImGui::End();
}