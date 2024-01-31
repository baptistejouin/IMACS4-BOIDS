#include "Environment/environment.hpp"

// constructor
Project::Environment::Environment()
    : radius{0.8f}
    , stroke_weight{0.005f}
    , center{0.f, 0.f}
    , background_color{0.7f, 0.7f, 0.7f}
    , fill_color{1.f, 1.f, 1.f}
{
}

// destructor
Project::Environment::~Environment()
{
}

void Project::Environment::draw_environment(p6::Context& ctx) const
{
    ctx.background(p6::Color{background_color});
    ctx.use_stroke    = true;
    ctx.use_fill      = true;
    ctx.stroke_weight = stroke_weight;
    ctx.fill          = {fill_color};
    ctx.square(p6::Center{center}, p6::Radius{radius});
}