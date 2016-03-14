#pragma once
//
// Created by andrey on 13.03.16.
//

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Camera {
    // global position (radius-vector)
    glm::vec3 pos;
    // up direction
    glm::vec3 up;
    // right direction
    glm::vec3 right;
    // where camera are looking at (a position)
    glm::vec3 cen;

    // polar system coordinate according to "cen"
    // angle fi
    float fi;
    // float angle pci
    float pci;
    // distance from "pos" to "cen"
    float r;
    // camera matrix (using for converting point
    // global coordinates to camera coordinates)
    glm::mat4x4 view_matrix;
protected:
    void update_view_matrix();
    void update_polar();
    void update_cartesian();
public:
    Camera(const glm::vec3 & init_pos,
           const glm::vec3 & init_up,
           const glm::vec3 & init_cen);

    // shift_<dir> - shift camera on direction <dir> on "step"
    void shift_up(const float step);
    void shift_down(const float step);
    void shift_right(const float step);
    void shift_left(const float step);

    // rotate_<dir> - rotate camera around "cen" on "step" angle
    void rotate_up(const float step);
    void rotate_down(const float step);
    void rotate_left(const float step);
    void rotate_right(const float step);

    // zoom_(in/out) - zoom in/out camera to/from "cen"
    void zoom_in(const float step);
    void zoom_out(const float step);

    const glm::mat4x4 & get_view_matrix() {
        return this->view_matrix;
    }

};