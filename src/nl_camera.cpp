#include <nl_camera.h>

void 
CreateViewMatrixFollow(Camera* camera, 
                       const v3f& position)
{
    const float left   = position.x - camera->size.x;
    const float right  = position.x + camera->size.x;
    const float bottom = position.y - camera->size.y;
    const float top    = position.y + camera->size.y;

    CreateOrtho(camera->view, left, right, bottom, top, 0.0f, 100.0f); 
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
