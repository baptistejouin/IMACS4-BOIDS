#include "Renderer/renderer.hpp"

Mesh::Mesh()
    : vbo()
    , vao()
    , shader(p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl"))
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
