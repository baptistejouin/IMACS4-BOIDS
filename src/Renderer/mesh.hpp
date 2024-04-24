#pragma once

#include <p6/p6.h>
#include <tiny_obj_loader.h>
#include <map>
#include "Utils/Geometry.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"

struct ShaderPaths {
    std::filesystem::path vertex_shader_path;
    std::filesystem::path fragment_shader_path;
};

class Mesh {
public:
    VBO                           vbo;
    VAO                           vao;
    p6::Shader                    shader;
    std::map<std::string, GLuint> uniform_variables;
    std::vector<ShapeVertex>      vertices;
    GLuint                        texture_id;
    // This constructor is used for meshes loaded from an obj file with a texture
    Mesh(
        const std::filesystem::path& obj_path,
        const std::filesystem::path& texture_path,
        const ShaderPaths&           shader_paths
    );

    // This constructor is used for other meshes that are not loaded from an obj file and do not have a texture and no light properties
    explicit Mesh(
        const std::vector<ShapeVertex>& vertices,
        const ShaderPaths&              shader_paths
    );

    void add_uniform_variable(const std::string& name);
    void add_light_uniforms(int lightIndex);
    void render_mesh() const;
    void change_mesh(const std::filesystem::path& obj_path = "", const std::filesystem::path& texture_path = "");

private:
    void setup_mesh();
};
