#include <nl_camera.h>

void 
CreateViewMatrixFollow(Camera* camera, 
                       const v3f& position)
{
    const float left   = position.x - camera->size.x;
    const float right  = position.x + camera->size.x;
    const float bottom = position.y - camera->size.y;
    const float top    = position.y + camera->size.y;

    camera->view = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

    CreateOrtho(camera->view, left, right, bottom, top, 0.0f, 100.0f); 
}
