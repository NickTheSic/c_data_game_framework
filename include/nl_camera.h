
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <nl_math.h>

struct Camera
{
    mat4f view;
    v2f size;
};

// Regular camera to follow a position passed in
void CreateViewMatrixFollow(Camera* camera, const v3f& position);

void GetMousePosInCamera(Camera* camera, v2f* out_pos, v2i mouse_pos);

// Possibly debug so I can copy cam2 into cam1
void CopyCameraToFrom(Camera* cam1, Camera* cam2);

#endif //_CAMERA_H_
