#pragma once

#include "Boids/boids.hpp"
#include "Environment/environment.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/IBO.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

class View {
public:
    View();

    void init(p6::Context& ctx);
    void update(p6::Context& ctx);

private:
    void             check_events(p6::Context& ctx);
    Environment      environment;
    Boids            boids;
    VBO              vbo;
    VAO              vao;
    const p6::Shader shader;
    const GLint      uMVPMatrixLocation;
    const GLint      uMVMatrixLocation;
    const GLint      uNormalMatrixLocation;
};
