
#ifndef NL_SPRITESHEET_H_
#define NL_SPRITESHEET_H_

#include <nl_math.h>
#include <nl_renderer.h>
#include <vector>
#include <map>

typedef int SpriteHandle;
static SpriteHandle INVALID_SPRITE_HANDLE = -1;

typedef void(*AnimEndCallback)(void *user_data);

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
void CleanupSpriteSheet(SpriteSheet* sheet);

SpriteHandle LoadSprite(SpriteSheet* sheet, const char* path);
void AddSpriteToRender(SpriteSheet* sheet, SpriteHandle spriteHandle, const v3f& pos);

void SpriteSheetBeginRender(SpriteSheet* sheet);
void SpriteSheetEndRender(SpriteSheet* sheet);

Sprite& GetSprite(SpriteHandle handle);

void InitializeSpriteAnim(SpriteAnimation* anim, int count, float speed = 1.0f);
void UpdateSpriteAnimation(SpriteAnimation* anim, float deltaTime);
void CleanupSpriteAnimation(SpriteAnimation* anim);
void RenderSpriteAnimationFrame(SpriteSheet* sheet, SpriteAnimation* anim, const v3f& pos);

// Debug to see what is currently loaded into texture memory 
// Currently only handles 1 texture ID 
void DisplayEntireSheet(SpriteSheet* sheet, const v3f& position, const v2f& size);


#endif //NL_SPRITESHEET_H_