#include "View/view.hpp"

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    ctx.imgui = [&]() {
        _environment.gui();
        _boids.gui();
    };

    ctx.mouse_dragged = [this](p6::MouseDrag const& drag) {
        {
            _camera.drag(drag.delta.y, drag.delta.x, 50.f);
        }
    };

    ctx.mouse_scrolled = [this](p6::MouseScroll const& scroll) {
        _camera.move_front(scroll.dy / 50);
    };

    auto test_model = Renderer::load_model("assets/models/oiseauBake.obj");
    if (test_model)
    {
        _models.push_back(std::move(test_model));
    }

    glEnable(GL_DEPTH_TEST);
}

void View::_check_events(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
}

void View::update(p6::Context& ctx)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _environment.draw(ctx);
    _renderer.render_boids(ctx, _camera, _boids.get_boids());
    _boids.update(ctx.delta_time());
    Renderer::render_model(ctx, _camera, *_models[0]);
    _check_events(ctx);
}
