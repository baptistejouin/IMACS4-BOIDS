#include "Environment/environment.hpp"

EnvironmentParams::EnvironmentParams()
    : background_color{0.7f, 0.7f, 0.7f}
    , terrain{{0.f, -1.75f, 0.f}, glm::vec3{0.3f}}
{
    const Light light_01 = {
        .index     = 0,
        .position  = {.5f, .25f, -.5f},
        .intensity = glm::vec3{2.f}
    };

    const Light light_02 = {
        .index     = 1,
        .position  = {.0f, -.9f, .5f},
        .intensity = glm::vec3{.25f}
    };

    point_light = {light_01, light_02};
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