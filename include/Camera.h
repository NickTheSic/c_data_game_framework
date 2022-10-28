
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "nl_math.h"

struct Camera
{
    mat4f view;
};

struct FollowCamera
{
    mat4f view;
    v3f* position;
};

static void CreateViewMatrix(Camera* camera, const v3f& position, const v3f& direction)
{
    camera->view = glm::lookAt(position, direction, v3f(0.f,1.f,0.f));
}

static void CreateFollowViewMatrix(FollowCamera* camera)
{
    if (camera->position != 0)
    {
        v3f pos = *camera->position;
        v3f dir = v3f(pos.x, pos.y, -10);
        camera->view = glm::lookAt(pos, dir, v3f(0.f,1.f,0.f));
    }
}

#endif //_CAMERA_H_
