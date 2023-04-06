#include <nl_camera.h>
#include <nl_debug.h>

void 
CreateViewMatrixFollow(Camera* camera, 
                       const v3f& position)
{
    const float left   = position.x - (camera->size.x + camera->zoom.x);
    const float right  = position.x + (camera->size.x + camera->zoom.x);
    const float bottom = position.y - (camera->size.y + camera->zoom.y);
    const float top    = position.y + (camera->size.y + camera->zoom.y);

    CreateOrtho(camera->view, left, right, bottom, top, 0.0f, 10.0f); 
}

void 
GetMousePosInCamera(Camera* camera, v2f* out_pos, v2i mouse_pos)
{
    const float camera_size_x = camera->size.x*2;
    const float camera_size_y = camera->size.y*2;

    out_pos->x = ((static_cast<float>(mouse_pos.x) - camera_size_x) * 0.5f);
    out_pos->y = ((static_cast<float>(mouse_pos.y) - camera_size_y) * 0.5f);

    LOG("Mouse In Camera X: %f, Y: %f", out_pos->x, out_pos->y);
}

void CopyCameraToFrom(Camera* cam1, Camera* cam2)
{
    cam1->view.m11 = cam2->view.m11;  cam1->view.m12 = cam2->view.m12;  
    cam1->view.m13 = cam2->view.m13; cam1->view.m14 = cam2->view.m14;
    cam1->view.m21 = cam2->view.m21;  cam1->view.m22 = cam2->view.m22;  
    cam1->view.m23 = cam2->view.m23; cam1->view.m24 = cam2->view.m24; 
    cam1->view.m31 = cam2->view.m31;  cam1->view.m32 = cam2->view.m32;  
    cam1->view.m33 = cam2->view.m33; cam1->view.m34 = cam2->view.m34; 
    cam1->view.m41 = cam2->view.m41;  cam1->view.m42 = cam2->view.m42;  
    cam1->view.m43 = cam2->view.m43; cam1->view.m44 = cam2->view.m44;

    cam1->size.x = cam2->size.x;
    cam1->size.y = cam2->size.y;
}

void ZoomInCamera(Camera* camera, bool zoom_out)
{
    const float adjust = zoom_out ? 10.f : -10.f;
    const float ratio = camera->size.x / camera->size.y;
    camera->zoom.x += adjust*ratio;
    camera->zoom.y += adjust;
}
