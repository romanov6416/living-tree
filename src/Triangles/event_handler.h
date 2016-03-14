//
// Created by andrey on 14.03.16.
//
#pragma once

//opengl headers
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera.h"

class EventHandler {
    // mouse coordinates
    int mouse_x;
    int mouse_y;
    // true if button is pressed
    bool right_button_active;
    Camera & camera;
public:
    EventHandler(Camera & camera);
    void active_mouse(int x_pos, int y_pos);
    void mouse(int button, int mode, int pos_x, int pos_y);
};



