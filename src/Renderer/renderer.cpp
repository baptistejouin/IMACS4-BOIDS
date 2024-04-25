#include "Renderer/renderer.hpp"
#include <imgui.h>

void setup_view_projection(p6::Context& ctx, Camera& camera, glm::mat4& ProjMatrix, glm::mat4& ViewMatrix)
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
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix));

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

    set_uniform_variable("uKd", glm::vec3(0.95f)); // diffuse
    set_uniform_variable("uKs", glm::vec3(0.95f)); // specular

    set_uniform_variable("uShininess", 100.f);

    for (size_t i = 0; i < point_light.size(); ++i)
    {
        auto light_position = glm::vec3(0.f);

        light_position = ViewMatrix * glm::vec4(point_light[i].position, 1.f);

        set_uniform_variable("uLightPos_vs[" + std::to_string(i) + "]", light_position);
        set_uniform_variable("uLightIntensity[" + std::to_string(i) + "]", point_light[i].intensity);
    }

    mesh.render_mesh();
}

void Renderer::render_boids(p6::Context& ctx, Camera& camera, const std::vector<Boid>& boids, const std::vector<Light>& point_light) const
{
    _boids_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& boid : boids)
    {
        // get the view matrix
        glm::mat4 ViewMatrix = camera.get_view_matrix();

        // move the boid to its position
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), boid.get_position());

        // scale the boid
        MVMatrix = glm::scale(MVMatrix, glm::vec3(boid.get_size()));

        // rotate the boid to face the direction it is going
        MVMatrix = glm::rotate(MVMatrix, boid.get_look_at_angle_and_axis().first, boid.get_look_at_angle_and_axis().second);

        finalize_rendering(_boids_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
    }
}

void Renderer::render_terrain(p6::Context& ctx, Camera& camera, const Element& terrain, const std::vector<Light>& point_light) const
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

void Renderer::render_flowers(p6::Context& ctx, Camera& camera, const std::vector<Element>& flowers, const std::vector<Light>& point_light) const
{
    _flower_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& flower : flowers)
    {
        // move the flower to its position
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), flower.position);

        // Limiter la plage de rotation entre -pi/2 et pi/2 radians
        float maxYRotation = glm::radians(90.0f); // Convertir 90 degrés en radians

        // Maintenant appliquer la rotation
        MVMatrix = glm::rotate(MVMatrix, glm::clamp(flower.rotation.y, -maxYRotation, maxYRotation), glm::vec3(0.f, 1.f, 0.f));

        // scale the flower
        MVMatrix = glm::scale(MVMatrix, flower.scale);

        finalize_rendering(_flower_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
    }
}

void Renderer::render_point_light(p6::Context& ctx, Camera& camera, const std::vector<Light>& point_light) const
{
    _point_light_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& light : point_light)
    {
        // move the light to its position
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), light.position);

        // scale the light
        MVMatrix = glm::scale(MVMatrix, glm::vec3(0.02f));

        finalize_rendering(_point_light_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
    }
}

void Renderer::render_arpenteur(p6::Context& ctx, Camera& camera, float scale, const std::vector<Light>& point_light) const
{
    _arpenteur_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    // move the arpenteur to its position
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), camera.get_position());

    // scale the arpenteur
    MVMatrix = glm::scale(MVMatrix, glm::vec3(scale));

    // rotate the arpenteur to face the direction of the camera

    finalize_rendering(_arpenteur_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
}

void Renderer::render_cube(p6::Context& ctx, Camera& camera, float scale, const std::vector<Light>& point_light) const
{
    _cube_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    // move the cube to its position
    glm::mat4 MVMatrix = glm::scale(glm::mat4(1.f), glm::vec3(scale));

    // scale the cube
    MVMatrix = glm::scale(MVMatrix, glm::vec3(scale));

    finalize_rendering(_cube_mesh, point_light, ProjMatrix, ViewMatrix, MVMatrix);
}

void Renderer::gui()
{
    ImGui::Begin("Renderer");

    if (ImGui::BeginCombo("Level Of Detail", _LOD)) // The second parameter is the label previewed before opening the combo.
    {
        if (ImGui::Selectable("High"))
        {
            _LOD = "High";
            _boids_mesh.change_mesh("assets/models/avion-HQ.obj");
        }
        if (ImGui::Selectable("Low"))
        {
            _LOD = "Low";
            _boids_mesh.change_mesh("assets/models/avion-LQ.obj");
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Type of flowers", "Blue"))
    {
        if (ImGui::Selectable("Blue"))
        {
            _flower_mesh.change_mesh("assets/models/flower_blue.obj");
        }
        if (ImGui::Selectable("Red"))
        {
            _flower_mesh.change_mesh("assets/models/flower_red.obj");
        }
        if (ImGui::Selectable("Yellow"))
        {
            _flower_mesh.change_mesh("assets/models/flower_yellow.obj");
        }
        if (ImGui::Selectable("White"))
        {
            _flower_mesh.change_mesh("assets/models/flower_white.obj");
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}
