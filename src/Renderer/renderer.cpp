#include "Renderer/renderer.hpp"

void setup_mesh(const Mesh& mesh)
{
    mesh.vbo.bind();
    mesh.vao.bind();

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, position)));

    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, normal)));

    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), reinterpret_cast<const GLvoid*>(offsetof(ShapeVertex, texCoords)));

    mesh.vbo.fill(mesh.vertices);

    mesh.vbo.unbind();
    mesh.vao.unbind();
}

void render_mesh(const Mesh& mesh)
{
    mesh.vao.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.texture_id);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertices.size()));

    glBindTexture(GL_TEXTURE_2D, 0);

    mesh.vao.unbind();
}

void setup_view_projection(p6::Context& ctx, TrackballCamera& camera, glm::mat4& ProjMatrix, glm::mat4& ViewMatrix)
{
    ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    ViewMatrix = camera.get_view_matrix();
}

void finalize_rendering(const Mesh& mesh, const glm::mat4& ProjMatrix, const glm::mat4& ViewMatrix, const glm::mat4& MVMatrix)
{
    // compute the normal matrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // compute the MVP matrix
    glm::mat4 MVPMatrix = ProjMatrix * ViewMatrix * MVMatrix;

    // send the matrices to the shader
    glUniform1i(mesh.uText, 0);
    glUniformMatrix4fv(mesh.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(mesh.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix * MVMatrix));
    glUniformMatrix4fv(mesh.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    render_mesh(mesh);
}

Mesh::Mesh(const std::filesystem::path& obj_path, const std::filesystem::path& texture_path, const ShaderPaths& shader_paths)
    : shader(p6::load_shader(shader_paths.vertex_shader_path, shader_paths.fragment_shader_path)), vertices(Renderer::load_model(obj_path)), texture_id(Renderer::load_texture(texture_path))
{
    uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");
    uText                 = glGetUniformLocation(shader.id(), "uText");

    setup_mesh(*this);
}

GLuint Renderer::load_texture(const std::filesystem::path& texture_path)
{
    if (texture_path.empty())
        return 0;

    // Load Image using p6
    img::Image textureImage = p6::load_image_buffer(texture_path);

    // Check if the image is loaded
    if (textureImage.data() == nullptr)
    {
        std::cerr << "Failed to load texture" << std::endl;
        exit(1);
    }

    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Generate the texture
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        static_cast<GLsizei>(textureImage.width()),
        static_cast<GLsizei>(textureImage.height()),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        textureImage.data()
    );

    // Set Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // TODO(baptiste): (optionnal) Add mipmaping support for the textures (with the distance of the camera to the object)
    // glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

std::vector<ShapeVertex> Renderer::load_model(const std::filesystem::path& obj_path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./assets/models";

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
                const auto& idx = shape.mesh.indices[fv * f + v];

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

    return vertices;
}

void Renderer::render_boids(p6::Context& ctx, TrackballCamera& camera, const std::vector<Boid>& boids) const
{
    _boids_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    for (auto const& boid : boids)
    {
        // get the view matrix
        glm::mat4 ViewMatrix = camera.get_view_matrix();

        // move the boid to its position
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), boid.get_position());

        // scale the boid
        MVMatrix = glm::scale(MVMatrix, glm::vec3(boid.get_size()));

        // rotate the boid to face the direction it is going
        MVMatrix = glm::rotate(MVMatrix, boid.get_look_at_angle_and_axis().first, boid.get_look_at_angle_and_axis().second);

        // by default the model is facing to the top, so we need to rotate it
        MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

        finalize_rendering(_boids_mesh, ProjMatrix, ViewMatrix, MVMatrix);
    }
}

void Renderer::render_terrain(p6::Context& ctx, TrackballCamera& camera, const Terrain& terrain) const
{
    _terrain_mesh.shader.use();

    glm::mat4 ProjMatrix, ViewMatrix;

    setup_view_projection(ctx, camera, ProjMatrix, ViewMatrix);

    // move the terrain to its position
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), terrain.position);

    // scale the terrain
    MVMatrix = glm::scale(MVMatrix, terrain.scale);

    finalize_rendering(_terrain_mesh, ProjMatrix, ViewMatrix, MVMatrix);
}