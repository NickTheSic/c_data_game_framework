
#ifndef NL_VIEWPORT_H
#define NL_VIEWPORT_H

#include <nl_math.h>

struct Viewport
{
    v2i screen_size;
    v2i screen_center;
};

void GetMouseInViewportWithCamera(v2f* out_pos, Viewport* viewport, Camera* camera, v2i mouse_pos);

#endif
