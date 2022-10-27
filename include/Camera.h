
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "nl_math.h"

struct Camera
{
    mat4f view;
};

static void CreateViewMatrix(Camera* camera, const v3f& position, const v3f& direction)
{
    camera->view = glm::lookAt(position, direction, v3f(0.f,1.f,0.f));
}

#endif //_CAMERA_H_
