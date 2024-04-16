#pragma once

#include <p6/p6.h>
#include <vector>
#include "Boids/boids.hpp"
#include "Camera/trackball.hpp"
#include "Environment/environment.hpp"
#include "Renderer/renderer.hpp"

class View {
public:
    View() = default;

    void init(p6::Context& ctx);
    void update(p6::Context& ctx);

private:
    void            _check_events(p6::Context& ctx);
    Environment     _environment;
    Boids           _boids;
    TrackballCamera _camera;
    Renderer        _renderer;
};
