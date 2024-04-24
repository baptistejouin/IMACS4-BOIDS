#include "Renderer/mesh.hpp"

GLuint load_texture(const std::filesystem::path& texture_path)
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

std::vector<ShapeVertex> load_model(const std::filesystem::path& obj_path)
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

Mesh::Mesh(const std::filesystem::path& obj_path, const std::filesystem::path& texture_path, const ShaderPaths& shader_paths)
    : shader(p6::load_shader(shader_paths.vertex_shader_path, shader_paths.fragment_shader_path)), vertices(load_model(obj_path)), texture_id(load_texture(texture_path))
{
    add_uniform_variable("uMVPMatrix");
    add_uniform_variable("uMVMatrix");
    add_uniform_variable("uNormalMatrix");

    // add the texture uniform
    add_uniform_variable("uText");

    // add the light uniform
    add_uniform_variable("uKd");
    add_uniform_variable("uKs");
    add_uniform_variable("uShininess");
    add_uniform_variable("uLightPos_vs");
    add_uniform_variable("uLightIntensity");

    setup_mesh();
}

Mesh::Mesh(const std::vector<ShapeVertex>& vertices, const ShaderPaths& shader_paths)
    : shader(p6::load_shader(shader_paths.vertex_shader_path, shader_paths.fragment_shader_path)), vertices(vertices), texture_id(0)
{
    add_uniform_variable("uMVPMatrix");
    add_uniform_variable("uMVMatrix");
    add_uniform_variable("uNormalMatrix");

    setup_mesh();
}

void Mesh::setup_mesh()
{
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

void Mesh::add_uniform_variable(const std::string& name)
{
    uniform_variables[name] = glGetUniformLocation(shader.id(), name.c_str());
}

void Mesh::render_mesh() const
{
    vao.bind();

    // if mesh has a texture
    if (texture_id != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

    if (texture_id != 0)
        glBindTexture(GL_TEXTURE_2D, 0);

    vao.unbind();
}
