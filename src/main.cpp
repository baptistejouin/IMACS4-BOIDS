#include <cstdlib>
#include "Boids/boids.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Environment/environment.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    Project::Environment environment;
    Project::Boids       boids{1000};

    // Declare your infinite update loop.
    ctx.update = [&]() {
        environment.draw_environment(ctx);
        boids.draw(ctx);
        boids.update();

        if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
        {
            ctx.stop();
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}