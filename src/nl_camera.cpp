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
    camera->view = glm::lookAt(position, dir, v3f(0.f,1.f,0.f));
}
