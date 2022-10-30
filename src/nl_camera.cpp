#include <nl_camera.h>

void CreateViewMatrix(Camera* camera, const v3f& position, const v3f& direction)
{
    camera->view = glm::lookAt(position, direction, v3f(0.f,1.f,0.f));
}

void CreateViewMatrixFollow(Camera* camera, const v3f& position)
{
    const v3f dir(position.x, position.y, -10);
    camera->view = glm::lookAt(position, dir, v3f(0.f,1.f,0.f));
}

void CreateFollowViewMatrix(FollowCamera* camera)
{
    if (camera->position != 0)
    {
        const v3f pos = *camera->position;
        v3f dir = v3f(pos.x, pos.y, -10);
        camera->view = glm::lookAt(pos, dir, v3f(0.f,1.f,0.f));
    }
}
