#include <nl_viewport.h>

void UpdateScreenSize(Viewport* viewport, int sx, int sy)
{
    viewport->screen_size.x = sx;
    viewport->screen_size.y = sy;
    viewport->screen_center.x = sx >> 2;
    viewport->screen_center.y = sy >> 2;
}

void GetMouseInViewportWithCamera(v2f* out_pos, Viewport* viewport, Camera* camera, v2i mouse_pos)
{
    const float camera_size_x = camera->size.x*2;
    const float camera_size_y = camera->size.y*2;

    out_pos->x = ((static_cast<float>(mouse_pos.x) - camera_size_x) * 0.5f) + viewport->screen_size.x;
    out_pos->y = ((static_cast<float>(mouse_pos.y) - camera_size_y) * 0.5f) + viewport->screen_size.y;
}
