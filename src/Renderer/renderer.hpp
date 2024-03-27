#pragma once

#include "Boids/boid.hpp"
#include "Camera/trackball.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

struct Mesh {
    VBO                      vbo;
    VAO                      vao;
    p6::Shader               shader;
    GLint                    uMVPMatrixLocation;
    GLint                    uMVMatrixLocation;
    GLint                    uNormalMatrixLocation;
    std::vector<ShapeVertex> vertices;

    Mesh();
};

class Renderer {
public:
    Renderer();

    void render_boids(p6::Context& ctx, TrackballCamera& camera, std::vector<Boid> boids) const;

private:
    Mesh _boids_mesh;
};