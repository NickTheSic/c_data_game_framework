/* date = October 22nd 2022 2:34 pm */

#ifndef MAIN_H
#define MAIN_H

struct v2f
{
    float x,y;
};

struct v2i
{
    int x,y;
};

struct v3f
{
    float x,y,z;
};

struct Colour
{
    unsigned char r,g,b,a;
};

struct SpriteVertexData
{
    v3f position;
    v2f uv;
    Colour colour;
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

struct GeneratedSprite
{
    int x,y,channel;
    unsigned char* data;
};

struct SpriteSheet
{
    Renderer renderer;
    
    v2i atlasSize;
    v2i currentAtlasLoc;
    int usedAtlasHeight;
    
    unsigned int textureID;
};


void InitializeSpriteSheet(SpriteSheet* sheet);
bool AddSprite(SpriteSheet* sheet, Sprite* sprite, const char* path);
void ClenupSpritesheet(SpriteSheet* sheet);

void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderCode);
void ComponeSpriteShaderProgram(Renderer* renderer);

void InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize);
void CleanupRenderer(Renderer* renderer);


#endif //MAIN_H
