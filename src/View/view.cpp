#include "View/view.hpp"

View::View()
{
    Environment environment;
    Boids       boids{1000};
}

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    ctx.imgui = [&]() {
        environment.gui();
    };
}

void View::check_events(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
}

void View::update(p6::Context& ctx)
{
    environment.draw(ctx);

    boids.draw(ctx);
    boids.update(ctx.delta_time());

    check_events(ctx);
}
