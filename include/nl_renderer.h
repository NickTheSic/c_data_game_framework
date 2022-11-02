/* date = October 23rd 2022 1:47 am */

#ifndef _NL_RENDERER_H
#define _NL_RENDERER_H

#include <nl_math.h>
#include <vector>
#include <map>

typedef int SpriteHandle;
static SpriteHandle INVALID_SPRITE_HANDLE = -1;
static v2f DEFAULT_SPRITE_SIZE = v2f{100.f,100.f};

typedef void(*AnimEndCallback)(void *userdata);

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

struct SpriteAnimation
{
    int count;
    float speed;
    float timed_index;
    AnimEndCallback callback;
    void *user_data;
    SpriteHandle* sprite_handles;
};

struct Renderer
{
    unsigned int vao, vbo, ebo;
    unsigned int shader_program;
    unsigned int vertex_count, max_vertices;
    unsigned int draw_count;
};

struct SpriteSheet
{
    Renderer renderer;
    
    v2i atlas_size;
    v2i current_atlas_loc;
    int used_atlas_height;
    
    unsigned int textureID;
    
    int sprite_count;
    std::map<std::string, SpriteHandle> loaded_sprites;
    std::vector<Sprite> sprites;
};

void InitializeSpriteSheet(SpriteSheet* sheet, int sx=1024, int sy=1024);
SpriteHandle LoadSprite(SpriteSheet* sheet, const char* path);
void AddSpriteToRender(SpriteSheet* sheet, SpriteHandle spriteHandle, const v3f& pos);
void BeginRender(SpriteSheet* sheet);

void DisplayEntireSheet(SpriteSheet* sheet, const v3f& position, const v2f& size);
void CleanupSpriteSheet(SpriteSheet* sheet);

void InitializeSpriteAnim(SpriteAnimation* anim, int count, float speed = 1.0f);
void UpdateSpriteAnimation(SpriteAnimation* anim, float deltaTime);
void CleanupSpriteAnimation(SpriteAnimation* anim);
void RenderSpriteAnimationFrame(SpriteSheet* sheet, SpriteAnimation* anim, const v3f& pos);

void InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize);
void CleanupRenderer(Renderer* renderer);
void BeginRender(Renderer* renderer);
void EndRender(Renderer* renderer);


#endif //_NL_RENDERER_H
