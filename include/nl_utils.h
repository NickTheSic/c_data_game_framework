#pragma once

#include <nl_math.h>
#include <nl_debug.h>


// TODO: Note, this is no longer bottom left and upper right as I am trying to make it 'just work'
// Refactor in the future
static inline bool
PointInRect(const v2f& point, const v2f& origin, const v2f& size)
{
    const bool ContainedWithin = 
            point.x > origin.x && point.x < origin.x + size.x && 
            point.y > origin.y && point.y < origin.y + size.y;
    
    return ContainedWithin;
}
