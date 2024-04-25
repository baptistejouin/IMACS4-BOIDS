#include "Environment/environment.hpp"
#include "Camera/camera.hpp"

glm::vec3 randomPointOnSurface()
{
    // Génération aléatoire des coordonnées dans le carré
    float x = p6::random::number(-1.0f, .5f);
    float z = p6::random::number(-.5f, 1.f);

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
        flower.rotation = glm::vec3{0.f, p6::random::number(0.f, 360.f), 0.f};
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
        .index     = 2,
        .position  = {.0f, -.1f, .0f}, // will be updated in update_fixed_light
        .intensity = glm::vec3{.5f}
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

void Environment::update_fixed_light(Camera& camera)
{
    _params.point_light[2].position = camera.get_position() + glm::vec3{0.f, -0.1f, 0.f};
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