#include "boids.hpp"
#include "glm/fwd.hpp"

// constructor
Boids::Boids(int nbBoids)
    : boids(nbBoids)
    , params{.turnFactor = 0.11f, .visualRange = .2f, .protectedRange = .06f, .centeringFactor = 0.005f, .avoidFactor = 0.5f, .matchingFactor = 0.5f, .maxSpeed = .3f, .minSpeed = .1}
{
}

void Boids::update(float delta_time)
{
    for (auto& boid : boids)
    {
        // Zero all accumulator variables (can't do this in one line in C)
        glm::vec2 posAvg           = {0, 0};
        glm::vec2 velAvg           = {0, 0};
        glm::vec2 closeD           = {0, 0};
        int       neighboringBoids = 0;

        // For every other boid in the flock
        for (auto const& otherBoid : boids)
        {
            if (&boid != &otherBoid)
            {
                // Calculate the distance between the two boids
                glm::vec2 d = boid.getPosition() - otherBoid.getPosition();

                if (std::fabs(d.x) < params.visualRange && std::fabs(d.y) < params.visualRange)
                {
                    float distance = glm::length(d);

                    // If the other boid is within the visual range
                    if (distance < params.visualRange)
                    {
                        // Add the position and velocity to the average
                        posAvg += otherBoid.getPosition();
                        velAvg += otherBoid.getVelocity();

                        // If the other boid is within the protected range
                        if (distance < params.protectedRange)
                        {
                            // Move away from the other boid
                            closeD += d;
                        }

                        // Increment the number of neighboring boids
                        neighboringBoids++;
                    }
                }
            }
        }

        if (neighboringBoids > 0)
        {
            // Divide accumulator variables by the number of neighboring boids
            posAvg /= neighboringBoids;
            velAvg /= neighboringBoids;

            // Add centering/matching forces to the boid's velocity
            boid.setVelocity(boid.getVelocity() + (posAvg - boid.getPosition()) * params.centeringFactor + (velAvg - boid.getVelocity()) * params.matchingFactor);
        }

        // Add avoidance force to the boid's velocity
        boid.setVelocity(boid.getVelocity() + closeD * params.avoidFactor);

        // If the boid is near an edge, make it turn by turnFactor
        if (boid.getPosition().x < -.8) // Square radius
        {
            boid.setVelocity(boid.getVelocity() + glm::vec2(params.turnFactor, 0));
        }
        if (boid.getPosition().x > .8)
        {
            boid.setVelocity(boid.getVelocity() + glm::vec2(-params.turnFactor, 0));
        }
        if (boid.getPosition().y < -.8)
        {
            boid.setVelocity(boid.getVelocity() + glm::vec2(0, params.turnFactor));
        }
        if (boid.getPosition().y > .8)
        {
            boid.setVelocity(boid.getVelocity() + glm::vec2(0, -params.turnFactor));
        }

        float speed = glm::length(boid.getVelocity());

        // Enforce minimum and maximum speeds
        if (speed > params.maxSpeed)
        {
            boid.setVelocity(boid.getVelocity() * (params.maxSpeed / speed));
        }
        else if (speed < params.minSpeed)
        {
            boid.setVelocity(boid.getVelocity() * (params.minSpeed / speed));
        }

        // Update the boid's position
        boid.update(delta_time, params);
    }
}

void Boids::draw(p6::Context& ctx) const
{
    for (auto const& boid : boids)
    {
        boid.draw(ctx);
    }
}

void Boids::reset()
{
    for (auto& boid : boids)
    {
        boid.set_position({0.f, 0.f});
    }
}

void Boids::gui()
{
    ImGui::Begin("Boids");
    ImGui::Text("Number of boids: %d", boids.size());
    if (ImGui::Button("Reset boids"))
    {
        reset();
    }
    ImGui::End();
}