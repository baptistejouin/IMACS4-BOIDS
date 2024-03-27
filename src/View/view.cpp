#include "View/view.hpp"

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    ctx.imgui = [&]() {
        this->environment.gui();
        this->boids.gui();
    };

    ctx.mouse_dragged = [this](p6::MouseDrag const& drag) {
        {
            this->camera.drag(drag.delta.y, drag.delta.x, 50.f);
        }
    };

    glEnable(GL_DEPTH_TEST);
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

    // this->environment.draw(ctx);
    this->boids.draw(ctx, this->camera);
    this->boids.update(ctx.delta_time());
    this->check_events(ctx);
}
