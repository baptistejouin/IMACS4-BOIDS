#include "Environment/environment.hpp"
#include "Camera/camera.hpp"

EnvironmentParams::EnvironmentParams()
    : background_color{0.03f, 0.15f, 0.22f}
    , terrain{{0.f, -1.75f, 0.f}, glm::vec3{0.3f}}
{
    const Light light_01 = {
        .index     = 0,
        .position  = {.5f, .25f, -.5f},
        .intensity = glm::vec3{.75f}
    };

    const Light light_02 = {
        .index     = 1,
        .position  = {.0f, -.9f, .5f},
        .intensity = glm::vec3{.25f}
    };

    const Light light_03 = {
        .index     = 99, // special index : the light moves with the camera
        .position  = {.0f, -.1f, .0f},
        .intensity = glm::vec3{.25f}
    };

    point_light = {light_01, light_02, light_03};
}

Environment::Environment(const EnvironmentParams& params)
    : _params(params)
{
}

void Environment::draw(p6::Context& ctx) const
{
    ctx.background(_params.background_color);
}

void Environment::gui()
{
    ImGui::Begin("Environment");
    ImGui::ColorEdit3("Background color", (float*)&_params.background_color);
    ImGui::End();
}