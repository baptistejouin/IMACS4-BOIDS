#include "Renderer/renderer.hpp"
#include <string>

Mesh::Mesh()
    : vbo()
    , vao()
    , shader(p6::load_shader("assets/shaders/3D.vs.glsl", "assets/shaders/normals.fs.glsl"))
    , vertices(Geometry::sphere_vertices(.01f, 32, 16))
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

void Renderer::render_boids(p6::Context& ctx, TrackballCamera& camera, std::vector<Boid> boids) const
{
    _boids_mesh.shader.use();

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
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glm::mat4 ViewMatrix   = camera.get_view_matrix();
        glm::mat4 MVPMatrix    = ProjMatrix * ViewMatrix * MVMatrix;

        glUniformMatrix4fv(_boids_mesh.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(_boids_mesh.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
        glUniformMatrix4fv(_boids_mesh.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        _boids_mesh.vao.bind();
        // think about texture here (binding)

        glDrawArrays(GL_TRIANGLES, 0, _boids_mesh.vertices.size());

        _boids_mesh.vao.unbind();
    }
}

void Renderer::render_model(p6::Context& ctx, TrackballCamera& camera, Mesh const& mesh)
{
    mesh.shader.use();

    glm::mat4 ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    glm::mat4 MVMatrix = glm::translate(
        glm::mat4(1.f),
        glm::vec3(0.f, 0.f, -2.f)
    );
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 ViewMatrix   = camera.get_view_matrix();
    glm::mat4 MVPMatrix    = ProjMatrix * ViewMatrix * MVMatrix;

    glUniformMatrix4fv(mesh.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(mesh.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
    glUniformMatrix4fv(mesh.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    mesh.vao.bind();
    // think about texture here (binding)

    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());

    mesh.vao.unbind();
}

std::unique_ptr<Mesh> Renderer::load_model(std::string const& obj_path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./assets/models"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(obj_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cerr << "TinyObjReader: " << reader.Warning();
    }

    const auto& attrib    = reader.GetAttrib();
    const auto& shapes    = reader.GetShapes();
    const auto& materials = reader.GetMaterials();

    std::vector<ShapeVertex> vertices;

    // Loop over shapes
    for (const auto& shape : shapes)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            auto fv = static_cast<size_t>(shape.mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                tinyobj::real_t  vx  = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index) + 0];
                tinyobj::real_t  vy  = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index) + 1];
                tinyobj::real_t  vz  = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index) + 2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * static_cast<size_t>(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * static_cast<size_t>(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * static_cast<size_t>(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * static_cast<size_t>(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * static_cast<size_t>(idx.texcoord_index) + 1];
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

                vertices.push_back({{vx, vy, vz}});
            }
            index_offset += fv;

            // per-face material
            shape.mesh.material_ids[f];
        }
    }

    auto mesh = std::make_unique<Mesh>();

    // temporary : fill mesh with vertices
    mesh->vbo.bind();
    mesh->vao.bind();

    mesh->vertices = vertices;
    mesh->vbo.fill(vertices);

    mesh->vbo.unbind();
    mesh->vao.unbind();

    return mesh;
}