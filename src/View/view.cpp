#include "View/view.hpp"
#include <cstdio>
#include "GLFW/glfw3.h"
#include "Utils/Probability.hpp"

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

    ctx.mouse_scrolled = [this](p6::MouseScroll const& scroll) {
        _camera.zoom(scroll.dy, 0.01f);
    };

    ctx.key_pressed = [this](p6::Key const& key) {
        if (key.logical == "F")
        {
            _environment.update_flowers_model_type();
        }
    };

    glEnable(GL_DEPTH_TEST);
}

void View::_check_events(p6::Context& ctx, Bounds bounds)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        _camera.move_front(0.01f, bounds);
    }
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        _camera.move_front(-0.01f, bounds);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        _camera.move_left(0.01f, bounds);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        _camera.move_left(-0.01f, bounds);
    }
    if (ctx.key_is_pressed(GLFW_KEY_SPACE))
    {
        _camera.move_up(0.01f, bounds);
    }
    if (ctx.key_is_pressed(GLFW_KEY_LEFT_SHIFT))
    {
        _camera.move_up(-0.01f, bounds);
    }
}

void View::update(p6::Context& ctx)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _environment.update_fixed_light(_camera);

    _environment.draw(ctx);
    _renderer.render_boids(ctx, _camera, _boids.get_boids(), _environment.get_points_light());
    _renderer.render_terrain(ctx, _camera, _environment.get_terrain(), _environment.get_points_light());
    _renderer.render_flowers(ctx, _camera, _environment.get_flowers(), _environment.get_points_light());
    _renderer.render_arpenteur(ctx, _camera, .03f, _environment.get_points_light());
    _renderer.render_cube(ctx, _camera, 1.2f, _environment.get_points_light());
    _renderer.render_point_light(ctx, _camera, _environment.get_points_light());

    // update flowers model type every 2 seconds
    // if (static_cast<int>(ctx.time()) % 2 == 0)
    // {
    //     // check if ctx.time is in a margin of 0.1 and 0.2 to avoid multiple call
    //     if (ctx.time() - static_cast<int>(ctx.time()) < 0.01f)
    //     {
    //         printf("%f Updating flowers model type\n", ctx.time());

    //     }
    // }

    _boids.update(ctx.delta_time());

    _check_events(ctx, _boids.get_bounds());
}
