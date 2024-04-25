#include "Environment/environment.hpp"
#include "Camera/camera.hpp"

glm::vec3 randomPointOnSurface()
{
    // Génération aléatoire des coordonnées dans le carré
    float x = p6::random::number(-1.45f, .40f);
    float z = p6::random::number(-1.25f, .6f);

    // La riviere est au centre de la map,
    // un bord 0.406678 -1.012720 0.210362
    // l'autre rive 0.355254 -1.009403 -0.290789
    // elle va de part en part de la map

    // Hauteur fixée
    float y = -1.05f;

    return {x, y, z};
}

std::vector<Element> randomFlowers(int n)
{
    std::vector<Element> flowers;
    for (int i = 0; i < n; i++)
    {
        Element flower;
        flower.position = randomPointOnSurface();
        flower.scale    = p6::random::number(0.01f, 0.05f) * glm::vec3{1.f};
        flowers.push_back(flower);
    }
    return flowers;
}

EnvironmentParams::EnvironmentParams()
    : background_color{0.035f, 0.105f, 0.141f}
    , terrain{{0.f, -1.75f, 0.f}, glm::vec3{0.4f}}
{
    const Light light_01 = {
        .index     = 0,
        .position  = {.5f, .25f, -.5f},
        .intensity = glm::vec3{.75f}
    };

    const Light light_02 = {
        .index     = 1,
        .position  = {.0f, -.8f, .5f},
        .intensity = glm::vec3{.25f}
    };

    const Light light_03 = {
        .index     = 99, // special index : the light moves with the camera
        .position  = {.0f, -.1f, .0f},
        .intensity = glm::vec3{.25f}
    };

    point_light = {light_01, light_02, light_03};

    // Element flower_corner_left_2 = {
    //     .position = {-1.45f, -1.22f, .6f},
    //     .scale    = glm::vec3{.05f}
    // };

    // Element flower_corner_right_2 = {
    //     .position = {.40f, -1.22f, .6f},
    //     .scale    = glm::vec3{.05f}
    // };

    // Element flower_corner_left_3 = {
    //     .position = {-1.45f, -1.22f, -1.25f},
    //     .scale    = glm::vec3{.05f}
    // };

    // Element flower_corner_right_3 = {
    //     .position = {.40f, -1.22f, -1.25f},
    //     .scale    = glm::vec3{.05f}
    // };

    flowers = randomFlowers(100);
}

Environment::Environment(const EnvironmentParams& params)
    : _params(params)
{
}

void Environment::draw(p6::Context& ctx) const
{
    ctx.background(_params.background_color);
}

void Environment::gui()
{
    ImGui::Begin("Environment");
    ImGui::ColorEdit3("Background color", (float*)&_params.background_color);
    ImGui::End();
}