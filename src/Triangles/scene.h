#pragma once
//
// Created by andrey on 13.03.16.
//

#include <glm/glm.hpp>
#include "camera.h"

class Scene {
    // matrices
    glm::mat4x4 model_matrix;
    glm::mat4x4 model_view_matrix;
    glm::mat4x4 proj_matrix;
    glm::mat4x4 model_view_proj_matrix;
    glm::mat4x4 norm_matrix;

    // camera
    Camera camera;
public:
    Scene();
    void draw();
};
