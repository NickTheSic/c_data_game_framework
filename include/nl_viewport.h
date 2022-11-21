
#ifndef NL_VIEWPORT_H
#define NL_VIEWPORT_H

#include <nl_math.h>

struct Viewport
{
    v2i screen_size;
    v2i screen_center;
};

v2f GetMouseInViewportWithCamera(Viewport* viewport, Camera *camera, v2i mouse_pos)
{
    const float camera_size_x = camera->size.x*2;
    const float camera_size_y = camera->size.y*2;
    
    float fpos_x = ((static_cast<float>(mouse_pos.x) - camera_size_x) * 0.5f) + viewport->screen_size.x;
    float fpos_y = ((static_cast<float>(mouse_pos.y) - camera_size_y) * 0.5f) + viewport->screen_size.y;

    return {fpos_x, fpos_y};
}

void GetMouseInViewportWithCamera(Viewport* viewport, Camera* camera, v2i mouse_pos, v2f* out_pos)
{
    const float camera_size_x = camera->size.x*2;
    const float camera_size_y = camera->size.y*2;

    out_pos->x = ((static_cast<float>(mouse_pos.x) - camera_size_x) * 0.5f) + viewport->screen_size.x;
    out_pos->y = ((static_cast<float>(mouse_pos.y) - camera_size_y) * 0.5f) + viewport->screen_size.y;
}

#endif
