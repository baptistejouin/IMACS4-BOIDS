#pragma once

#include "Boids/boids.hpp"
#include "Camera/trackball.hpp"
#include "Environment/environment.hpp"
#include "p6/p6.h"

class View {
public:
    View() = default;

    void init(p6::Context& ctx);
    void update(p6::Context& ctx);

private:
    void            check_events(p6::Context& ctx);
    Environment     environment;
    Boids           boids;
    TrackballCamera camera;
};
