#include <nl_camera.h>

void 
CreateViewMatrix(Camera* camera, 
                 const v3f& position, 
                 const v3f& direction)
{
    camera->view = glm::lookAt(position, direction, v3f(0.f,1.f,0.f));
}

void 
CreateViewMatrixFollow(Camera* camera, 
                       const v3f& position, 
                       float dir_offset /*=-10.f*/)
{
    const v3f dir(position.x, position.y, dir_offset);

    const float left   = position.x - camera->size.x;
    const float right  = position.x + camera->size.x;
    const float bottom = position.y - camera->size.y;
    const float top    = position.y + camera->size.y;

    camera->view = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);
}
