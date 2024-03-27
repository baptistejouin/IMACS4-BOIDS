#include "boids.hpp"
#include "glm/fwd.hpp"

// constructor
Boids::Boids(int nbBoids)
    : boids(nbBoids)
    , shader(p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl"))
    , uMVPMatrixLocation(glGetUniformLocation(shader.id(), "uMVPMatrix"))
    , uMVMatrixLocation(glGetUniformLocation(shader.id(), "uMVMatrix"))
    , uNormalMatrixLocation(glGetUniformLocation(shader.id(), "uNormalMatrix"))
    , vertices(Geometry::sphere_vertices(1.f, 32, 16))
{
    vbo.bind();
    vao.bind();

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, position)));

    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, normal)));

    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, texCoords)));

    vbo.fill(vertices);

    vbo.unbind();
    vao.unbind();
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

                    if (distance < params.protectedRange)
                    {
                        closeD += d / distance * (params.protectedRange - distance);
                    }
                    // If the other boid is within the visual range
                    else if (distance < params.visualRange)
                    {
                        // Add the position and velocity to the average
                        posAvg += otherBoid.getPosition();
                        velAvg += otherBoid.getVelocity();

                        // Increment the number of neighboring boids
                        neighboringBoids++;
                    }
                }
            }
        }

        glm::vec2 centeringForce = {0, 0};
        glm::vec2 matchingForce  = {0, 0};
        glm::vec2 avoidanceForce = {0, 0};

        if (neighboringBoids > 0)
        {
            // Divide accumulator variables by the number of neighboring boids
            posAvg /= neighboringBoids;
            velAvg /= neighboringBoids;

            // Calcul de la différence entre la position moyenne des voisins et la position actuelle du boid
            centeringForce = (posAvg - boid.getPosition()) * params.centeringFactor;

            // Calcul de la différence entre la vitesse moyenne des voisins et la vitesse actuelle du boid
            matchingForce = (velAvg - boid.getVelocity()) * params.matchingFactor;
        }
        // Add avoidance force to the boid's velocity
        avoidanceForce = closeD * params.avoidFactor;

        boid.setVelocity(boid.getVelocity() + avoidanceForce + centeringForce + matchingForce);

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
        boid.update(delta_time);
    }
}

void Boids::draw(p6::Context& ctx, TrackballCamera& camera) const
{
    shader.use();

    glm::mat4 ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    for (auto const& boid : boids)
    {
        boid.draw(vao, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, vertices, camera);
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
    ImGui::SliderFloat("Turn factor", &params.turnFactor, .001f, .5f);
    ImGui::SliderFloat("Visual range", &params.visualRange, .001f, .5f);
    ImGui::SliderFloat("Protected range", &params.protectedRange, .001f, .5f);
    ImGui::SliderFloat("Centering factor", &params.centeringFactor, .001f, .5f);
    ImGui::SliderFloat("Avoid factor", &params.avoidFactor, .001f, 1.f);
    ImGui::SliderFloat("Matching factor", &params.matchingFactor, .001f, 1.f);
    ImGui::SliderFloat("Max speed", &params.maxSpeed, .001f, 1.f);
    ImGui::SliderFloat("Min speed", &params.minSpeed, .001f, 1.f);
    ImGui::End();
}