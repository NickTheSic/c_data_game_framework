
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

void CreateViewMatrix(Camera* camera, const v3f& position, const v3f& direction);
// Regular camera to follow a position passed in
void CreateViewMatrixFollow(Camera* camera, const v3f& position);
// Handles a Follow camera object
void CreateFollowViewMatrix(FollowCamera* camera);

#endif //_CAMERA_H_
