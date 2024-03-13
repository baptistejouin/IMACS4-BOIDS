#pragma once

struct BoidsParams {
    float turnFactor{0.11f};
    float visualRange{.2f};
    float protectedRange{.06f};
    float centeringFactor{.005f};
    float avoidFactor{.5f};
    float matchingFactor{.5f};
    float maxSpeed{.3f};
    float minSpeed{.1f};
};