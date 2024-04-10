#pragma once

#include <tiny_obj_loader.h>
#include <cmath>
#include <string>
#include "Boids/boid.hpp"
#include "Camera/trackball.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

struct ShaderPaths {
    std::filesystem::path vertex_shader_path;
    std::filesystem::path fragment_shader_path;
};
struct Mesh {
    VBO                      vbo;
    VAO                      vao;
    p6::Shader               shader;
    GLint                    uMVPMatrixLocation;
    GLint                    uMVMatrixLocation;
    GLint                    uNormalMatrixLocation;
    std::vector<ShapeVertex> vertices;

    explicit Mesh(const std::vector<ShapeVertex>& custom_vertices, const ShaderPaths& shader_paths = ShaderPaths{"assets/shaders/3D.vs.glsl", "assets/shaders/normals.fs.glsl"});
    Mesh(); // default constructor
};
class Renderer {
public:
    Renderer() = default;

    static std::unique_ptr<Mesh> load_model(const std::filesystem::path& obj_path);
    void                         render_boids(p6::Context& ctx, TrackballCamera& camera, const std::vector<Boid>& boids) const;

private:
    std::unique_ptr<Mesh> _boids_mesh = load_model("assets/models/oiseauBake.obj");
};