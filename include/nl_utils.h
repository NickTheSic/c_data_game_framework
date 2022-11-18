#pragma once

#include <nl_math.h>

static inline bool 
PointInRect(v2f point, v2f bl_coord, v2f ur_coord)
{
    return point.x > bl_coord.x && point.x < ur_coord.x
        && point.y > bl_coord.y && point.y < ur_coord.y;
}
