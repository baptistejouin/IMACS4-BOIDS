#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "Camera/trackball.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/IBO.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"
#include "boidsParams.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

class Boids {
public:
    Boids(int nbBoids = 20);

    // methods
    void update(float delta_time);
    void draw(p6::Context& ctx, TrackballCamera& camera) const;
    void addBoid();
    void removeBoid();
    void reset();
    void gui();

private:
    std::vector<Boid>              boids;
    BoidsParams                    params;
    VBO                            vbo;
    VAO                            vao;
    const p6::Shader               shader;
    const GLint                    uMVPMatrixLocation;
    const GLint                    uMVMatrixLocation;
    const GLint                    uNormalMatrixLocation;
    const std::vector<ShapeVertex> vertices;
};