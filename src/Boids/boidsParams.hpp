#pragma once

struct BoidsParams {
    float turnFactor{0.03f};
    float visualRange{.15f};
    float protectedRange{.065f};
    float centeringFactor{.001f};
    float avoidFactor{.9f};
    float matchingFactor{.02f};
    float maxSpeed{.8f};
    float minSpeed{.5f};
};