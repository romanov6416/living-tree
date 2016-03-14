//
// Created by andrey on 14.03.16.
//

#include "event_handler.h"

void EventHandler::active_mouse(int x_pos, int y_pos) {
    if (right_button_active) {
        mouse_x = x_pos;
        mouse_y = y_pos;
    }

}

void EventHandler::mouse(int button, int mode, int x_pos, int y_pos) {
    if (button == GLUT_RIGHT_BUTTON)
        right_button_active = mode == 1;
}

EventHandler::EventHandler(Camera &cam) :
        camera(cam) {

}
