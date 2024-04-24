#include "View/view.hpp"

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    ctx.imgui = [&]() {
        _environment.gui();
        _boids.gui();
        _renderer.gui();
    };

    ctx.mouse_dragged = [this](p6::MouseDrag const& drag) {
        {
            _camera.rotate_left(-drag.delta.x * 300);
            _camera.rotate_up(drag.delta.y * 300);
        }
    };

    glEnable(GL_DEPTH_TEST);
}

void View::_check_events(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        _camera.move_front(0.01f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        _camera.move_front(-0.01f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        _camera.move_left(0.01f);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        _camera.move_left(-0.01f);
    }
}

void View::update(p6::Context& ctx)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _environment.draw(ctx);
    _renderer.render_boids(ctx, _camera, _boids.get_boids(), _environment.get_points_light());
    _renderer.render_terrain(ctx, _camera, _environment.get_terrain(), _environment.get_points_light());
    _renderer.render_flowers(ctx, _camera, _environment.get_flowers(), _environment.get_points_light());
    _renderer.render_arpenteur(ctx, _camera, .03f, _environment.get_points_light());
    _renderer.render_cube(ctx, _camera, 1.2f, _environment.get_points_light());
    _renderer.render_point_light(ctx, _camera, _environment.get_points_light());
    _boids.update(ctx.delta_time());
    _check_events(ctx);
}
