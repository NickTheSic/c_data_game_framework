#include <nl_framework.h>

void
InitializeFramework(Framework* fw, v2i sheet_size, v2f camera_size, v3f starting_camera_position, unsigned int batch_count)
{
    fw->main_camera.size.x = camera_size.x;
    fw->main_camera.size.y = camera_size.y;

    InitializeRenderer(&fw->sprite_sheet.renderer, batch_count, sizeof(SpriteVertexData));
    InitializeSpriteSheet(&fw->sprite_sheet, sheet_size.x, sheet_size.y);
    CreateViewMatrixFollow(&fw->main_camera, starting_camera_position);
    CompileSpriteShaderProgram(&fw->sprite_sheet.renderer);
    fw->shader.program = fw->sprite_sheet.renderer.shader_program;
    //SetUniform(&fw->shader, "view", fw->main_camera.view);
    SetViewUniform(&fw->shader, fw->main_camera.view);
}

void
CleanupFramework(Framework* fw)
{
    CleanupRenderer(&fw->sprite_sheet.renderer);
    CleanupSpriteSheet(&fw->sprite_sheet);
}
