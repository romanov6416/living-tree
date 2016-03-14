//
// Created by andrey on 13.03.16.
//
#include "camera.h"

Camera * Camera::instance = nullptr;

void Camera::shift_up(const float step) {
    this->pos += step * this->up;
    this->cen += step * this->up;
    this->update_polar();
    this->update_view_matrix();
}
void Camera::shift_down(const float step) {
    this->pos -= step * this->up;
    this->cen -= step * this->up;
    this->update_polar();
    this->update_view_matrix();
}

void Camera::shift_left(const float step) {
//    glm::vec3 right = this->get_right();
    this->pos -= step * this->right;
    this->cen -= step * this->right;
    this->update_polar();
    this->update_view_matrix();
}

void Camera::shift_right(const float step) {
    this->pos += step * this->right;
    this->cen += step * this->right;
    this->update_polar();
    this->update_view_matrix();
}

void Camera::rotate_up(const float step) {
    pci += step;
    while (pci > M_PI/2)
        pci -= 2*M_PI;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::rotate_down(const float step) {
    pci -= step;
    while (pci < -M_PI)
        pci += 2*M_PI;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::rotate_left(const float step) {
    fi -= step;
    while (fi < -M_PI)
        fi += 2*M_PI;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::rotate_right(const float step) {
    fi += step;
    while (fi > -M_PI)
        fi -= 2*M_PI;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::zoom_in(const float step) {
    if (r - step > 0.0f)
        r -= step;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::zoom_out(const float step) {
    r += step;
    this->update_cartesian();
    this->update_view_matrix();
}

void Camera::update_view_matrix() {
    this->view_matrix = glm::lookAt(pos, cen, up);
}

void Camera::update_cartesian() {
    double cen_dir_x = r * cos(pci) * cos(fi);
    double cen_dir_z = r * cos(pci) * sin(fi);
    double cen_dir_y = r * sin(pci);
    pos = cen + glm::vec3(cen_dir_x, cen_dir_y, cen_dir_z);
    double up_dir_x = cos(pci + M_PI/2) * cos(fi);
    double up_dir_z = cos(pci + M_PI/2) * sin(fi);
    double up_dir_y = sin(pci + M_PI/2);
    up = glm::normalize(glm::vec3(up_dir_x, up_dir_y, up_dir_z));
    right = glm::normalize(glm::cross(cen - pos, up));
}

void Camera::update_polar() {
    float x = pos.x - cen.x;
    float y = pos.y - cen.y;
    float z = pos.z - cen.z;
    r = static_cast<float>(sqrt(x*x + y*y + z*z));
    fi = static_cast<float>(atan2(z, x));
    pci = static_cast<float>(asin(y / r));
}

Camera::Camera(const glm::vec3 & init_pos, const glm::vec3 & init_up,
               const glm::vec3 & init_cen) :
        pos(init_pos), cen(init_cen) {
    if (instance == nullptr)
        instance = this;
    this->up = glm::normalize(init_up);
    this->right = glm::normalize(glm::cross(cen - pos, up));
    this->update_polar();
    this->update_view_matrix();
}

Camera * Camera::get_instance() {
    return Camera::instance;
}

Camera::~Camera() {
    this->instance = nullptr;
}
