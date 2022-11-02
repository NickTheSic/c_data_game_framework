
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "nl_math.h"

struct Camera
{
    mat4f view;
    v2f size;
};

void CreateViewMatrix(Camera* camera, const v3f& position, const v3f& direction);
// Regular camera to follow a position passed in
void CreateViewMatrixFollow(Camera* camera, const v3f& position, float dir_offset = -10.f);

#endif //_CAMERA_H_
