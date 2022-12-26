#ifndef _NL_COLLISION_H_
#define _NL_COLLISION_H_

#include <nl_math.h>
// I could return a float for the amount of overlap?
// Since physics wise I may want to know how to bounce or stop

bool RectOverlapRect(v2f origin1, v2f size1, v2f origin2, v2f size2);
bool RectContainsRect(v2f origin1, v2f size1, v2f origin2, v2f size2);

bool CircleOverlapsCircle(float origin1, float radius1, float origin2, float radius2);
bool CircleContainsCircle(float origin1, float radius1, float origin2, float radius2);

bool RectContainsCircle();

#endif //_NL_COLLISION_H_