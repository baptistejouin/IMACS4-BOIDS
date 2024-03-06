#include "Environment/environment.hpp"

// constructor
Environment::Environment(const EnvironmentParams& params)
    : p{params}
{
}

void Environment::draw(p6::Context& ctx) const
{
    ctx.background(p6::Color{p.background_color});
    ctx.use_stroke    = true;
    ctx.use_fill      = true;
    ctx.stroke_weight = p.stroke_weight;
    ctx.fill          = {p.fill_color};
    ctx.square(p6::Center{p.center}, p6::Radius{p.radius});
}

void Environment::gui()
{
    ImGui::Begin("Environment");
    ImGui::SliderFloat("Square size", &p.radius, 0.5f, 1.5f);
    ImGui::ColorEdit3("Background color", reinterpret_cast<float*>(&p.background_color));
    ImGui::ColorEdit3("Fill color", reinterpret_cast<float*>(&p.fill_color));
    ImGui::End();
    // ImGui::ShowDemoWindow();
}