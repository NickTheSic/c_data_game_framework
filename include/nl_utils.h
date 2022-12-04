#pragma once

#include <nl_math.h>
#include <nl_debug.h>

static inline bool
PointInRect(const v2f& point, const v2f& origin, const v2f& size)
{
    const bool ContainedWithin = 
            point.x > origin.x && point.x < origin.x + size.x && 
            point.y > origin.y && point.y < origin.y + size.y;

    //LOG("Point X: %f, Y: %f was%s contained in origin X: %f, Y: %f, Size X: %f, Y: %f", \
        point.x, point.y, ContainedWithin ? "" : "not", origin.x, origin.y, origin.x+size.x, origin.y+size.y);
    
    return ContainedWithin;
}
