#include <nl_viewport.h>
#include <nl_camera.h>
#include <nl_math.h>

void 
UpdateScreenSize(Viewport* viewport, int sx, int sy)
{
    viewport->screen_size.x = sx;
    viewport->screen_size.y = sy;
    //LOG("Viewport size x: %d, viewport size y: %d",  viewport->screen_size.x, viewport->screen_size.y);

    //Maybe also want half screen size

    //Are't used anywhere else at the moment
    viewport->screen_center.x = sx - (sx / 2);
    viewport->screen_center.y = sy - (sy / 2);

    //LOG("Viewport screen center x: %d, center y: %d", viewport->screen_center.x, viewport->screen_center.y)
}

void 
GetMouseInViewportWithCamera(v2f* out_pos, Viewport* viewport, Camera* camera, v2i mouse_pos)
{
    const float camera_size_x = camera->size.x*2;
    const float camera_size_y = camera->size.y*2;

    LOG("Camera size x: %f, y: %f", camera_size_x, camera_size_y);

    // TODO: Fix this...?
    // Didn't work for the UI viewport/camera
    // Not sure if screen size is the correct addition now
    out_pos->x = ((static_cast<float>(mouse_pos.x) - camera_size_x) * 0.5f) + viewport->screen_size.x;
    out_pos->y = ((static_cast<float>(mouse_pos.y) - camera_size_y) * 0.5f) + viewport->screen_size.y;

    LOG("Mouse out pos x: %f, y: %f", out_pos->x, out_pos->y);
}

void 
GetScreenSize(Viewport* viewport, int* sx, int* sy)
{
    // Assume if we are getting screen size we actually passed in values
    *sx = viewport->screen_size.x;
    *sy = viewport->screen_size.y;
}
