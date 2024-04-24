#pragma once

#include <p6/p6.h>
#include <tiny_obj_loader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Boids/boid.hpp"
#include "Camera/camera.hpp"
#include "Environment/environment.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"

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
    GLint                    uText;
    std::vector<ShapeVertex> vertices;
    GLuint                   texture_id;

    explicit Mesh(
        const std::filesystem::path& obj_path,
        const std::filesystem::path& texture_path,
        const ShaderPaths&           shader_paths
    );
};

class Renderer {
public:
    Renderer() = default;

    static GLuint                   load_texture(const std::filesystem::path& texture_path);
    static std::vector<ShapeVertex> load_model(const std::filesystem::path& obj_path);

    void render_boids(p6::Context& ctx, Camera& camera, const std::vector<Boid>& boids) const;
    void render_terrain(p6::Context& ctx, Camera& camera, const Terrain& terrain) const;
    void render_arpenteur(p6::Context& ctx, Camera& camera, float scale) const;

private:
    Mesh _boids_mesh = Mesh(
        "assets/models/oiseauBake.obj",
        "assets/textures/oiseauBake.jpg",
        {"assets/shaders/3D.vs.glsl", "assets/shaders/textures.fs.glsl"}
    );
    Mesh _terrain_mesh = Mesh(
        "assets/models/terrain.bake.obj",
        "assets/textures/terrain.bake.col.png",
        {"assets/shaders/3D.vs.glsl", "assets/shaders/textures.fs.glsl"}
    );
    Mesh _arpenteur_mesh = Mesh(
        "assets/models/ballons.obj",
        "assets/textures/ballons.png",
        {"assets/shaders/3D.vs.glsl", "assets/shaders/textures.fs.glsl"}
    );
};