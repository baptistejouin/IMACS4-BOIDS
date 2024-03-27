#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Boids/boid.hpp"
#include "Camera/trackball.hpp"
#include "Renderer/renderer.hpp"
#include "Utils/Geometry.hpp"
#include "Utils/IBO.hpp"
#include "Utils/VAO.hpp"
#include "Utils/VBO.hpp"
#include "boidsParams.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

class Boids {
public:
    Boids(int nbBoids = 500);

    void update(float delta_time);
    void reset();
    void gui();

    std::vector<Boid> get_boids();

private:
    std::vector<Boid> _boids;
    BoidsParams       _params;
};