#include "View/view.hpp"

View::View()
    : shader(p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl")), uMVPMatrixLocation(glGetUniformLocation(shader.id(), "uMVPMatrix")), uMVMatrixLocation(glGetUniformLocation(shader.id(), "uMVMatrix")), uNormalMatrixLocation(glGetUniformLocation(shader.id(), "uNormalMatrix"))
{
    Environment environment;
    Boids       boids;
    VBO         vbo{};
    VAO         vao{};
}

void View::init(p6::Context& ctx)
{
    ctx.maximize_window();

    // 🚧 DEBUT refacto 🚧

    // Bind the VBO
    vbo.bind();

    glEnable(GL_DEPTH_TEST);

    // 🚧 FIN refacto 🚧

    ctx.imgui = [&]() {
        environment.gui();
        boids.gui();
    };
}

void View::check_events(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_ESCAPE))
    {
        ctx.stop();
    }
}

void View::update(p6::Context& ctx)
{
    // 🚧 DEBUT refacto 🚧

    shader.use();

    glm::mat4 ProjMatrix = glm::perspective(
        glm::radians(70.f),
        ctx.aspect_ratio(),
        0.1f,
        100.f
    );

    glm::mat4 MVMatrix = glm::translate(
        glm::mat4(1.f),
        glm::vec3(0.f, 0.f, -5.f)
    );

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    vao.bind();

    const std::vector<ShapeVertex> vertices = Geometry::sphere_vertices(1.f, 32, 16);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    vao.unbind();

    // 🚧 FIN refacto 🚧

    // environment.draw(ctx);
    // boids.draw(ctx);
    // boids.update(ctx.delta_time());
    // check_events(ctx);
}
