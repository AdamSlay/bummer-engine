#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class Camera {
public:
    Camera();
    void center_on_object(const SDL_Rect& object_rect, int X_OFFSET, int Y_OFFSET);
    void reset();
    SDL_Rect getCameraRect() const;

private:
    float calculate_smoothing(float obj_distance, float smoothing, float range_limit, float dead_zone);

    int origin_x;
    int origin_y;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int GROUND_LEVEL_Y;
    SDL_Rect camera_rect;
};

#endif // CAMERA_H