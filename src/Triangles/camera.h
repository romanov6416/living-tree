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
    // where camera are looking at (a position)
    glm::vec3 cen;
    // camera matrix (using for converting point
    // global coordinates to camera coordinates)
    glm::mat4x4 view_matrix;

    const glm::vec3 & get_right();
protected:
    void update_view_matrix();
public:
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

};