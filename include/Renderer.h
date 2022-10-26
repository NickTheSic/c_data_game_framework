/* date = October 23rd 2022 1:47 am */

#ifndef _RENDERER_H
#define _RENDERER_H

#include <Types.h>
#include <vector>

struct SpriteVertexData
{
    v3f position;
    v2f uv;
};

struct Sprite
{
    v2f size;
    v2f bl_coord, ur_coord;
};

typedef int SpriteHandle;
static SpriteHandle INVALID_SPRITE_HANDLE = -1;
static v2f DEFAULT_SPRITE_SIZE = v2f{0.2f,0.2f};

typedef void(*AnimEndCallback)();

struct SpriteAnimation
{
    int count;
    float speed;
    float timed_index;
    AnimEndCallback callback;
    SpriteHandle* sprite_handles;
};

struct Renderer
{
    unsigned int vbo, vao, ebo;
    unsigned int vertex_count, max_vertices;
    unsigned int draw_count;
    unsigned int shader_program;
};

struct SpriteSheet
{
    Renderer renderer;
    
    v2i atlas_size;
    v2i current_atlas_loc;
    int used_atlas_height;
    
    unsigned int textureID;

    int sprite_count;
    std::vector<Sprite> sprites;
};


void InitializeSpriteSheet(SpriteSheet* sheet, int sx=1024, int sy=1024);
SpriteHandle LoadSprite(SpriteSheet* sheet, const char* path);
void AddSpriteToRender(SpriteSheet* sheet, SpriteHandle spriteHandle, const v3f& pos);
void EndRender(Renderer* renderer);
void DisplayEntireSheet(SpriteSheet* sheet, const v3f& position, const v2f& size);
void CleanupSpritesheet(SpriteSheet* sheet);

void InitializeSpriteAnim(SpriteAnimation* anim, int count, float speed = 1.0f);
void UpdateSpriteAnimation(SpriteAnimation* anim, float deltaTime);
void CleanupSpriteAnimation(SpriteAnimation* anim);

void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode);
void CompileSpriteShaderProgram(Renderer* renderer);

void InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize);
void CleanupRenderer(Renderer* renderer);




#endif //_RENDERER_H
