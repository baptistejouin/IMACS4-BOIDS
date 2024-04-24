#pragma once

#include <p6/p6.h>
#include <tiny_obj_loader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Boids/boid.hpp"
#include "Camera/trackball.hpp"
#include "Environment/environment.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"

struct ShaderPaths {
    std::filesystem::path vertex_shader_path;
    std::filesystem::path fragment_shader_path;
};
struct Mesh {
    VBO                          vbo;
    VAO                          vao;
    p6::Shader                   shader;
    std::map<std::string, GLint> uniformVariables;
    std::vector<ShapeVertex>     vertices;
    GLuint                       texture_id;

public:
    // This constructor is used for meshes loaded from an obj file with a texture
    Mesh(
        const std::filesystem::path& obj_path,
        const std::filesystem::path& texture_path,
        const ShaderPaths&           shader_paths
    );

    // This constructor is used for other meshes that are not loaded from an obj file and do not have a texture
    explicit Mesh(
        const std::vector<ShapeVertex>& vertices,
        const ShaderPaths&              shader_paths
    );

    void add_uniform_variable(const std::string& name);
    void render_mesh() const;

private:
    void setup_mesh();
};

class Renderer {
public:
    Renderer() = default;

    static GLuint                   load_texture(const std::filesystem::path& texture_path);
    static std::vector<ShapeVertex> load_model(const std::filesystem::path& obj_path);

    // TODO(baptistejouin) ctx is pass only for the aspect ratio, maybe we can find a better way to do it
    void render_boids(p6::Context& ctx, TrackballCamera& camera, const std::vector<Boid>& boids, const std::vector<Element>& point_light) const;
    void render_terrain(p6::Context& ctx, TrackballCamera& camera, const Element& terrain, const std::vector<Element>& point_light) const;
    void render_point_light(p6::Context& ctx, TrackballCamera& camera, const std::vector<Element>& point_light) const;

private:
    // fs : point_light, textures, normals

    Mesh _boids_mesh = Mesh(
        "assets/models/oiseauBake.obj",
        "assets/textures/oiseauBake.jpg",
        {"assets/shaders/3D.vs.glsl", "assets/shaders/point_light.fs.glsl"}
    );
    Mesh _terrain_mesh = Mesh(
        "assets/models/terrain.bake.obj",
        "assets/textures/terrain.bake.col.png",
        {"assets/shaders/3D.vs.glsl", "assets/shaders/point_light.fs.glsl"}
    );

    Mesh _point_light_mesh = Mesh(
        Geometry::sphere_vertices(1.0f, 10, 10),
        {"assets/shaders/3D.vs.glsl", "assets/shaders/normals.fs.glsl"}
    );
};