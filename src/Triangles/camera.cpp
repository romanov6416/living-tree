//
// Created by andrey on 13.03.16.
//

#include "camera.h"

//
//void updateCameraPlace(CameraDirection direction)
//{
//    const float STEP_VER = M_PI / 36;
//    const float STEP_HOR = M_PI / 36;
//    const float MIN_R = 1.5f;
//    const float MAX_R = 200.0f;
//
//    float x = eye.x - cen.x;
//    float y = eye.y - cen.y;
//    float z = eye.z - cen.z;
//
//    float r = sqrt(x*x + y*y + z*z);
//    float fi = atan2(z, x);
//    float pci = asin(y / r);
//    if (up.y < 0)
//    {
//        fi += fi > 0 ? -M_PI : M_PI;
//        pci = pci > 0 ? M_PI - pci : -M_PI - pci;
//    }
//
//    switch (direction)
//    {
//        case ZOOM_OUT:
//            if (r < MAX_R) ++r;
//            break;
//        case ZOOM_IN:
//            if (r > MIN_R) --r;
//            break;
//        case UP:
//            pci += STEP_VER;
//            if (pci > M_PI) pci -=2*M_PI;
//            break;
//        case DOWN:
//            pci -= STEP_VER;
//            if (pci < -M_PI) pci += 2*M_PI;
//            break;
//        case LEFT:
//            fi += STEP_HOR;
//            if (fi > M_PI) fi -= 2*M_PI;
//            break;
//        case RIGHT:
//            fi -= STEP_HOR;
//            if (fi < -M_PI) fi += 2*M_PI;
//            break;
//    }
//    eye.x = cen.x + r * cos(pci) * cos(fi);
//    eye.z = cen.z + r * cos(pci) * sin(fi);
//    eye.y = cen.y + r * sin(pci);
//    up.x = cos(pci + M_PI/2) * cos(fi);
//    up.z = cos(pci + M_PI/2) * sin(fi);
//    up.y = sin(pci + M_PI/2);

void Camera::shift_up(const float step) {
    this->pos += step * this->up;
    this->cen += step * this->up;
    this->update_view_matrix;
}
void Camera::shift_down(const float step) {
    this->pos -= step * this->up;
    this->cen -= step * this->up;
    this->update_view_matrix;
}

void Camera::shift_left(const float step) {
    glm::vec3 right = this->get_right();
    this->pos -= step * right;
    this->cen -= step * right;
    this->update_view_matrix;
}

void Camera::shift_right(const float step) {
    glm::vec3 right = this->get_right();
    this->pos += step * right;
    this->cen += step * right;
    this->update_view_matrix;
}

void Camera::update_view_matrix() {
    this->view_matrix = glm::lookAt(pos, cen, up);
}

const glm::vec3 & Camera::get_right() {
    glm::vec3 center_dir = this->cen - this->pos;
    glm::vec3 right = glm::cross(center_dir, up);
    glm::vec3 norm_right = glm::normalize(right);
    return norm_right;
}
