
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "nl_math.h"

struct Camera
{
    mat4f view;
    v2f size;
};

// Regular camera to follow a position passed in
void CreateViewMatrixFollow(Camera* camera, const v3f& position);

#endif //_CAMERA_H_
