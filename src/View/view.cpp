#include "View/view.hpp"

View::View()
{
    Environment environment;
    Boids       boids;
}

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    ctx.imgui = [&]() {
        environment.gui();
        boids.gui();
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // environment.draw(ctx);
    boids.draw(ctx);
    // boids.update(ctx.delta_time());
    check_events(ctx);
}
