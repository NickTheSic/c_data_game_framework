
#ifndef NL_FRAMEWORK_H_
#define NL_FRAMEWORK_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_renderer.h>
#include <nl_shader.h>
#include <nl_spritesheet.h>

// Should the UI be part of the framework?
struct Framework
{
    Camera main_camera;
    SpriteSheet sprite_sheet;
    Shader shader;
};

void InitializeFramework(Framework* fw, v2i sheet_size, v2f camera_size, v3f starting_camera_position, unsigned int batch_count);
void CleanupFramework(Framework* fw);

#endif //NL_FRAMEWORK_H_
