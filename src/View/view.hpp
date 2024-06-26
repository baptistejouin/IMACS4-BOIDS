#pragma once

#include <p6/p6.h>
#include "Boids/boids.hpp"
#include "Camera/camera.hpp"
#include "Environment/environment.hpp"
#include "Renderer/renderer.hpp"

class View {
public:
    View() = default;

    void init(p6::Context& ctx);
    void update(p6::Context& ctx);

private:
    void        _check_events(p6::Context& ctx, Bounds bounds);
    Environment _environment;
    Boids       _boids;
    Camera      _camera;
    Renderer    _renderer;
};
