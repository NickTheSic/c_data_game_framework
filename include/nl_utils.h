#pragma once

#include <nl_math.h>
#include <nl_debug.h>


// TODO: Note, this is no longer bottom left and upper right as I am trying to make it 'just work'
// Refactor in the future
static inline bool 
PointInRect(v2f point, v2f bl_coord, v2f ur_coord)
{
    const bool ContainedWithin = 
            point.x > bl_coord.x && point.x < ur_coord.x + bl_coord.x && 
            point.y > bl_coord.y && point.y < ur_coord.y + bl_coord.y;
    
    static int count; ++count;

    if (count > 100){
    LOG("Point X: %f, Y: %f is %s the points %f, %f and %f, %f\n", 
    point.x, point.y, 
    ContainedWithin ? "within" : "not within",
    bl_coord.x, bl_coord.y,
    ur_coord.x, ur_coord.y);
    }

    return ContainedWithin;
}
