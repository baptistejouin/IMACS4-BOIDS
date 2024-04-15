#pragma once

#include <imgui.h>
#include <p6/p6.h>

struct EnvironmentParams {
    p6::Color background_color{0.7f, 0.7f, 0.7f};
};

class Environment {
public:
    explicit Environment(const EnvironmentParams& params = {});

    void draw(p6::Context& ctx) const;
    void gui();

private:
    EnvironmentParams p;
};
