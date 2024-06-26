#define TINYOBJLOADER_IMPLEMENTATION
#include <p6/p6.h>
#include "View/view.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    auto ctx = p6::Context{{.title = "Boids 3D - DEVAUX, JOUIN"}};

    View view;

    view.init(ctx);

    ctx.update = [&]() {
        view.update(ctx);
    };

    ctx.start();

    return EXIT_SUCCESS;
}
