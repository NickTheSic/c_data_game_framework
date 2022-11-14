
#ifndef NL_FRAMEWORK_H_
#define NL_FRAMEWORK_H_

#include <nl_camera.h>
#include <nl_math.h>
#include <nl_renderer.h>
#include <nl_shader.h>

struct Framework
{
    Camera main_camera;
    SpriteSheet sprite_sheet;
    Shader shader;
};

struct FrameworkInitializationData
{
    v2f camera_size;
    v2f initialize_camera_position;
    unsigned int sheet_width, sheet_height;
    unsigned int batch_count;
};

#endif //NL_FRAMEWORK_H_
