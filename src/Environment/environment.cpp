#include "Environment/environment.hpp"

Environment::Environment(const EnvironmentParams& params)
    : p{params}
{
}

void Environment::draw(p6::Context& ctx) const
{
    ctx.background(p.background_color);
}

void Environment::gui()
{
    ImGui::Begin("Environment");
    ImGui::ColorEdit3("Background color", (float*)&p.background_color);
    ImGui::End();
}