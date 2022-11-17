#include <nl_framework.h>

Framework* 
InitializeFramework(const FrameworkInitializationData& fw_data)
{
    // new rewquired for Shader std::map
    Framework* fw = new Framework;

    fw->main_camera.size.x = fw_data.camera_size.x;
    fw->main_camera.size.y = fw_data.camera_size.y;

    InitializeRenderer(&fw->sprite_sheet.renderer, fw_data.batch_count, sizeof(SpriteVertexData));
    InitializeSpriteSheet(&fw->sprite_sheet, fw_data.sheet_size.x, fw_data.sheet_size.y);
    CreateViewMatrixFollow(&fw->main_camera, fw_data.starting_camera_position);
    CompileSpriteShaderProgram(&fw->sprite_sheet.renderer);
    fw->shader.program = fw->sprite_sheet.renderer.shader_program;
    SetUniform(&fw->shader, "view", fw->main_camera.view);

    return fw;
}

void
CleanupFramework(Framework* fw)
{
    CleanupRenderer(&fw->sprite_sheet.renderer);
    CleanupSpriteSheet(&fw->sprite_sheet);
    
    delete fw;
}
