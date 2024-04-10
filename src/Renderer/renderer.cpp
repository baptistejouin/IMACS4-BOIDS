#include "Renderer/renderer.hpp"

Mesh::Mesh(const std::vector<ShapeVertex>& custom_vertices, const ShaderPaths& shader_paths)
    : shader(p6::load_shader(shader_paths.vertex_shader_path, shader_paths.fragment_shader_path)), vertices(custom_vertices)
{
    uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    vbo.bind();
    vao.bind();

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, position)));

    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, normal)));

    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, texCoords)));

    vbo.fill(vertices);

    vbo.unbind();
    vao.unbind();
}

Mesh::Mesh()
    : Mesh(Geometry::sphere_vertices(.01f, 32, 16)) {} // the default mesh is a sphere with default shaders

void Renderer::render_boids(p6::Context& ctx, TrackballCamera& camera, std::vector<Boid> boids) const
{
    _boids_mesh->shader.use();

    glm::mat4 ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    for (auto const& boid : boids)
    {
        glm::mat4 MVMatrix = glm::translate(
            glm::mat4(1.f),
            glm::vec3(boid.get_position())
        );

        // scale down the boid
        MVMatrix = glm::scale(MVMatrix, glm::vec3(.04f));

        // rotate the boid with the velocity
        glm::vec3 velocity = boid.get_velocity();
        glm::vec3 up       = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 axis     = glm::cross(up, velocity);
        float     angle    = glm::acos(glm::dot(up, velocity) / (glm::length(up) * glm::length(velocity)));

        MVMatrix = glm::rotate(MVMatrix, angle, axis);

        // by default the model is facing to the top, so we need to rotate it
        MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glm::mat4 ViewMatrix   = camera.get_view_matrix();
        glm::mat4 MVPMatrix    = ProjMatrix * ViewMatrix * MVMatrix;

        glUniformMatrix4fv(_boids_mesh->uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(_boids_mesh->uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
        glUniformMatrix4fv(_boids_mesh->uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        _boids_mesh->vao.bind();
        // think about texture here (binding)

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_boids_mesh->vertices.size()));

        _boids_mesh->vao.unbind();
    }
}

std::unique_ptr<Mesh> Renderer::load_model(const std::filesystem::path& obj_path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./assets/models"; // Chemin vers les fichiers de matériaux

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(obj_path, reader_config))
    {
        std::cerr << "TinyObjReader: " << reader.Error();
        exit(1);
    }

    if (!reader.Warning().empty())
        std::cerr << "TinyObjReader: " << reader.Warning();

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    std::vector<ShapeVertex> vertices;

    for (const auto& shape : shapes)
    {
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            size_t fv = shape.mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; ++v)
            {
                auto& idx = shape.mesh.indices[fv * f + v];

                auto vx = attrib.vertices[3 * idx.vertex_index + 0];
                auto vy = attrib.vertices[3 * idx.vertex_index + 1];
                auto vz = attrib.vertices[3 * idx.vertex_index + 2];

                auto nx = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 0] : 0.0f;
                auto ny = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 1] : 0.0f;
                auto nz = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 2] : 0.0f;

                auto tx = (idx.texcoord_index >= 0) ? attrib.texcoords[2 * idx.texcoord_index + 0] : 0.0f;
                auto ty = (idx.texcoord_index >= 0) ? attrib.texcoords[2 * idx.texcoord_index + 1] : 0.0f;

                vertices.emplace_back(ShapeVertex{{vx, vy, vz}, {nx, ny, nz}, {tx, ty}});
            }
        }
    }

    return std::make_unique<Mesh>(vertices);
}
