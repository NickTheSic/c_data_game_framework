/* date = October 23rd 2022 1:47 am */

#ifndef _RENDERER_H
#define _RENDERER_H

#include <Types.h>

struct SpriteVertexData
{
    v3f position;
    v2f uv;
    //Colour colour;
};

struct Sprite
{
    v3f position;
    v2f size;
    v2f bl_coord, ur_coord;
    Colour colour;
};

struct Renderer
{
    unsigned int vbo, vao, ebo;
    unsigned int vertexCount, maxVertices;
    unsigned int drawCount;
    unsigned int shaderProgram;
};

struct SpriteSheet
{
    Renderer renderer;
    
    v2i atlasSize;
    v2i currentAtlasLoc;
    int usedAtlasHeight;
    
    unsigned int textureID;
};


void InitializeSpriteSheet(SpriteSheet* sheet, int sx=1024, int sy=1024);
bool LoadSprite(SpriteSheet* sheet, Sprite* sprite, const char* path);
void AddSpriteToRender(SpriteSheet* sheet, Sprite* sprite);
void EndRender(Renderer* renderer);
void CleanupSpritesheet(SpriteSheet* sheet);

void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode);

void CompileSpriteShaderProgram(Renderer* renderer);

void InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize);

void CleanupRenderer(Renderer* renderer);




#endif //_RENDERER_H
