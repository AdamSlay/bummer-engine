#include "Camera.h"
#include <cmath>
#include <fstream>
#include "../Config.h"

Camera::Camera() {
    origin_x = CAMERA_X;
    origin_y = CAMERA_Y;
    camera_rect = {origin_x, origin_y, SCREEN_WIDTH, SCREEN_HEIGHT};
}

float Camera::calculate_smoothing(float obj_distance, float smoothing, float range_limit, float dead_zone) {
    if (obj_distance < dead_zone) {
        return 0.0f;
    }
    if (obj_distance < range_limit) {
        return smoothing * (obj_distance / range_limit);
    }
    return smoothing;
}

void Camera::center_on_object(const SDL_Rect& object_rect, const int X_OFFSET, const int Y_OFFSET) {
    int desired_x = (object_rect.x + object_rect.w / 2) - X_OFFSET;
    int desired_y = (object_rect.y + object_rect.h / 2) - Y_OFFSET;

    float obj_distance_x = std::abs(object_rect.x - desired_x);
    float obj_distance_y = std::abs(object_rect.y - desired_y);

    float smoothing_x = calculate_smoothing(obj_distance_x, 0.05f, 100.0f, 0.0f);
    float smoothing_y = calculate_smoothing(obj_distance_y, 0.05f, 100.0f, 0.0f);

    camera_rect.x += (desired_x - camera_rect.x) * smoothing_x;
    camera_rect.y += (desired_y - camera_rect.y) * smoothing_y;
}

void Camera::reset() {
    camera_rect = {origin_x, origin_y, SCREEN_WIDTH, SCREEN_HEIGHT};
}

SDL_Rect Camera::getCameraRect() const {
    return camera_rect;
}