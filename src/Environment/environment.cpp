#include "Environment/environment.hpp"

Environment::Environment(const EnvironmentParams& params)
    : p{params}
{
}

void Environment::draw(p6::Context& ctx) const
{
    ctx.background(p6::Color{p.background_color});
}

void Environment::gui()
{
    ImGui::Begin("Environment");
    ImGui::ColorEdit3("Background color", reinterpret_cast<float*>(&p.background_color));
    ImGui::End();
}