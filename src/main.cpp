#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
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

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::Color{.7f, .7f, .7f});

        // Carré bordure
        ctx.use_stroke    = true;
        ctx.use_fill      = true;
        ctx.stroke_weight = .005f;
        ctx.fill          = {1.f, 1.f, 1.f};
        ctx.square(p6::Center{0, 0}, p6::Radius{.8f});

        // Cercle de la souris
        ctx.use_fill   = true;
        ctx.use_stroke = false;
        ctx.fill       = {1.f, 1.f, 1.f};
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.05f}
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}