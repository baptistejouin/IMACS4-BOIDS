#include "Renderer/renderer.hpp"

{
void setup_view_projection(p6::Context& ctx, Camera& camera, glm::mat4& ProjMatrix, glm::mat4& ViewMatrix)
void setup_view_projection(p6::Context& ctx, TrackballCamera& camera, glm::mat4& ProjMatrix, glm::mat4& ViewMatrix)
{
    ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    ViewMatrix = camera.get_view_matrix();
}

void finalize_rendering(const Mesh& mesh, const std::vector<Light>& point_light, const glm::mat4& ProjMatrix, const glm::mat4& ViewMatrix, const glm::mat4& MVMatrix)
{
    // compute the normal matrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // compute the MVP matrix
    glm::mat4 MVPMatrix = ProjMatrix * ViewMatrix * MVMatrix;

    // send the matrices to the shader
    glUniformMatrix4fv(mesh.uniform_variables.at("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(mesh.uniform_variables.at("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
    glUniformMatrix4fv(mesh.uniform_variables.at("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    auto set_uniform_variable = [&](const std::string& uniformName, auto value) {
        if (auto iter = mesh.uniform_variables.find(uniformName); iter != mesh.uniform_variables.end())
        {
            if constexpr (std::is_same_v<decltype(value), int>)
                glUniform1i(iter->second, value);
            else if constexpr (std::is_same_v<decltype(value), float>)
                glUniform1f(iter->second, value);
            else if constexpr (std::is_same_v<decltype(value), glm::vec3>)
                glUniform3fv(iter->second, 1, glm::value_ptr(value));
            else
            {
                throw std::runtime_error("Unsupported type");
            }
        }
    };

    set_uniform_variable("uText", 0);

    set_uniform_variable("uKd", glm::vec3(0.95f));
    set_uniform_variable("uKd", glm::vec3(0.95f));
    set_uniform_variable("uKs", glm::vec3(0.95f));

    set_uniform_variable("uShininess", 100.f);

    for (size_t i = 0; i < point_light.size(); ++i)
    {
        const glm::vec3 light_position = ViewMatrix * glm::vec4(point_light[i].position, 1.f);

        set_uniform_variable("uLightPos_vs[" + std::to_string(i) + "]", light_position);
        set_uniform_variable("uLightIntensity[" + std::to_string(i) + "]", point_light[i].intensity);
    }

    mesh.render_mesh();
}

void Renderer::render_boids(p6::Context& ctx, TrackballCamera& camera, const std::vector<Boid>& boids, const std::vector<Light>& point_light) const
{
    _boids_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& boid : boids)
    {
        // get the view matrix
        glm::mat4 ViewMatrix = camera.get_view_matrix();

void finalize_rendering(const Mesh& mesh, const glm::mat4& ProjMatrix, const glm::mat4& ViewMatrix, const glm::mat4& MVMatrix)
{
    // compute the normal matrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // compute the MVP matrix
    glm::mat4 MVPMatrix = ProjMatrix * ViewMatrix * MVMatrix;

    // send the matrices to the shader
    glUniform1i(mesh.uText, 0);
    glUniformMatrix4fv(mesh.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(mesh.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
    glUniformMatrix4fv(mesh.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    render_mesh(mesh);
}

        finalize_rendering(_boids_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
    }
}

void Renderer::render_terrain(p6::Context& ctx, TrackballCamera& camera, const Element& terrain, const std::vector<Light>& point_light) const
{
    _terrain_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    // move the terrain to its position
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), terrain.position);

    // scale the terrain
    MVMatrix = glm::scale(MVMatrix, terrain.scale);

    finalize_rendering(_terrain_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
}

void Renderer::render_point_light(p6::Context& ctx, TrackballCamera& camera, const std::vector<Light>& point_light) const
{
    _point_light_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& light : point_light)
    {
        // move the light to its position
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), light.position);

        // scale the light
        MVMatrix = glm::scale(MVMatrix, glm::vec3(0.1f));

        finalize_rendering(_point_light_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
    }
void Renderer::render_arpenteur(p6::Context& ctx, Camera& camera, float scale) const
    MVMatrix = glm::scale(MVMatrix, glm::vec3(scale));

    // scale the arpenteur
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), camera.get_position());
    // move the arpenteur to its position
    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);



    glm::mat4 ProjMatrix, ViewMatrix;
{
    _arpenteur_mesh.shader.use();

    // rotate the arpenteur to face the direction of the camera

    finalize_rendering(_arpenteur_mesh, ProjMatrix, ViewMatrix, MVMatrix);
void Renderer::render_terrain(p6::Context& ctx, Camera& camera, const Terrain& terrain) const
{
    _terrain_mesh.shader.use();


    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);


}
    finalize_rendering(_terrain_mesh, ProjMatrix, ViewMatrix, MVMatrix);

    // scale the terrain
    MVMatrix = glm::scale(MVMatrix, terrain.scale);

    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), terrain.position);
    // move the terrain to its position
    glm::mat4 ProjMatrix, ViewMatrix;

}
        finalize_rendering(_boids_mesh, ProjMatrix, ViewMatrix, MVMatrix);
    }
        MVMatrix = glm::rotate(MVMatrix, boid.get_look_at_angle_and_axis().first, boid.get_look_at_angle_and_axis().second);


        // rotate the boid to face the direction it is going
        // scale the boid
        MVMatrix = glm::scale(MVMatrix, glm::vec3(boid.get_size()));

        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), boid.get_position());
        // move the boid to its position

        glm::mat4 ViewMatrix = camera.get_view_matrix();
        // get the view matrix
    {
    for (auto const& boid : boids)

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    glm::mat4 ProjMatrix, ViewMatrix;

    _boids_mesh.shader.use();
{
void Renderer::render_boids(p6::Context& ctx, Camera& camera, const std::vector<Boid>& boids) const
}