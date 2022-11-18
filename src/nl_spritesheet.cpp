#include <nl_spritesheet.h>

#include <nl_math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

struct GeneratedSprite
{
    int x,y,channel;
    unsigned char* data;
};

void 
InitializeSpriteSheet(SpriteSheet *sheet, int sx, int sy)
{
    stbi_set_flip_vertically_on_load(true);
    
    sheet->atlas_size.x = sx;
    sheet->atlas_size.y = sy;
    
    // uneeded as long as we = {}; before passing?
    sheet->sprite_count = 0;
    sheet->used_atlas_height = 0;
    sheet->current_atlas_loc.x = 0;
    sheet->current_atlas_loc.y = 0;
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &sheet->textureID);
    glBindTexture(GL_TEXTURE_2D, sheet->textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 sheet->atlas_size.x,
                 sheet->atlas_size.y,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 0
                 );
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, position)));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexData), (void*)(offsetof(SpriteVertexData, uv)));
    glEnableVertexAttribArray(1);
}

SpriteHandle LoadSprite(SpriteSheet* sheet, const char* path)
{
    auto it = sheet->loaded_sprites.find(path);
    
    if (it != sheet->loaded_sprites.end())
    {
        return it->second;
    }
    
    Sprite sprite = {};
    SpriteHandle handle = INVALID_SPRITE_HANDLE;
    
    GeneratedSprite gsd = {};
    
    gsd.data = stbi_load(path, &gsd.x, &gsd.y, &gsd.channel, 4);
    
    if (stbi_failure_reason())
    {
        std::cout << stbi_failure_reason() << " while loading: " << path << std::endl;
        return handle;
    }
    
    if ((sheet->current_atlas_loc.x + gsd.x) > sheet->atlas_size.x)
    {
        sheet->current_atlas_loc.x = 0; 
        sheet->current_atlas_loc.y = sheet->used_atlas_height;
    }
    
    if (sheet->current_atlas_loc.y + gsd.y > sheet->atlas_size.y)
    {
        fprintf(stderr, "No more height in the current spritesheet to add sprite.\n");
        return handle;
    }
    
    sprite.bl_coord.x = (float)sheet->current_atlas_loc.x / (float)sheet->atlas_size.x;
    sprite.bl_coord.y = (float)sheet->current_atlas_loc.y / (float)sheet->atlas_size.y;
    sprite.ur_coord.x = (float)(sheet->current_atlas_loc.x + gsd.x) / (float)sheet->atlas_size.x;
    sprite.ur_coord.y = (float)(sheet->current_atlas_loc.y + gsd.y) / (float)sheet->atlas_size.y;
    
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    sheet->current_atlas_loc.x, sheet->current_atlas_loc.y,
                    gsd.x, gsd.y, 
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    gsd.data);
    
    sheet->current_atlas_loc.x += gsd.x;
    sheet->used_atlas_height = (sheet->used_atlas_height - sheet->current_atlas_loc.y) < gsd.y
        ? gsd.y + sheet->current_atlas_loc.y : sheet->used_atlas_height;
    
    stbi_image_free(gsd.data);
    
    handle = sheet->sprite_count++;
    sprite.size.x = 32;
    sprite.size.y = 32;
    //sprite.size = gsd;
    sheet->sprites.push_back(sprite);
    
    sheet->loaded_sprites[path] = handle;
    
    return sheet->loaded_sprites[path];
}

void
AddSpriteToRender(SpriteSheet* sheet, SpriteHandle spriteHandle, const v3f& pos)
{
    if (spriteHandle == INVALID_SPRITE_HANDLE)
    {
        //fprintf(stderr, "Sprite Handle was Invalid\n");
        return;
    }
    else if (spriteHandle > sheet->sprite_count || spriteHandle < 0)
    {
        fprintf(stderr, "Sprite Handle was %d which is not in range of the sheet sprites", spriteHandle);
        return;
    }
    
    if (sheet->renderer.vertex_count + 4 > sheet->renderer.max_vertices)
    {
        EndRender(&sheet->renderer);
    }
    
    const Sprite& sprite = sheet->sprites[spriteHandle];
    
    SpriteVertexData vertices[4] = {};
    v3f& pos0 = vertices[0].position;
    pos0.x = pos.x;
    pos0.y = pos.y;
    pos0.z = pos.z;
    
    v3f& pos1 = vertices[1].position;
    pos1.x = pos.x + sprite.size.x;
    pos1.y = pos.y;
    pos1.z = pos.z;
    
    v3f& pos2 = vertices[2].position;
    pos2.x = pos.x + sprite.size.x;
    pos2.y = pos.y + sprite.size.y;
    pos2.z = pos.z;
    
    v3f& pos3 = vertices[3].position;
    pos3.x = pos.x;
    pos3.y = pos.y + sprite.size.y;
    pos3.z = pos.z;
    
    v2f& uv0 = vertices[0].uv;
    uv0.x = sprite.bl_coord.x;
    uv0.y = sprite.bl_coord.y;
    
    v2f& uv1 = vertices[1].uv;
    uv1.x = sprite.ur_coord.x;
    uv1.y = sprite.bl_coord.y;
    
    v2f& uv2 = vertices[2].uv;
    uv2.x = sprite.ur_coord.x;
    uv2.y = sprite.ur_coord.y;
    
    v2f& uv3 = vertices[3].uv;
    uv3.x = sprite.bl_coord.x;
    uv3.y = sprite.ur_coord.y;
    
    glBufferSubData(GL_ARRAY_BUFFER,
                    sheet->renderer.vertex_count*sizeof(SpriteVertexData),
                    4*sizeof(SpriteVertexData),
                    &vertices[0]);
    
    sheet->renderer.vertex_count += 4;
    ++sheet->renderer.draw_count;
}

void
DisplayEntireSheet(SpriteSheet *sheet, const v3f& pos, const v2f& size)
{
    if (sheet->renderer.vertex_count > sheet->renderer.max_vertices)
    {
        EndRender(&sheet->renderer);
    }
    
    SpriteVertexData vertices[4] = {};
    
    v3f& pos0 = vertices[0].position;
    pos0.x = pos.x;
    pos0.y = pos.y;
    pos0.z = pos.z;
    
    v3f& pos1 = vertices[1].position;
    pos1.x = pos.x + size.x;
    pos1.y = pos.y;
    pos1.z = pos.z;
    
    v3f& pos2 = vertices[2].position;
    pos2.x = pos.x + size.x;
    pos2.y = pos.y + size.y;
    pos2.z = pos.z;
    
    v3f& pos3 = vertices[3].position;
    pos3.x = pos.x;
    pos3.y = pos.y + size.y;
    pos3.z = pos.z;
    
    v2f& uv0 = vertices[0].uv;
    uv0.x = 0.0f; uv0.y = 0.0f;
    v2f& uv1 = vertices[1].uv;
    uv1.x = 1.0f; uv1.y = 0.0f;
    v2f& uv2 = vertices[2].uv;
    uv2.x = 1.0f; uv2.y = 1.0f;
    v2f& uv3 = vertices[3].uv;
    uv3.x = 0.0f; uv3.y = 1.0f;
    
    glBufferSubData(GL_ARRAY_BUFFER, sheet->renderer.vertex_count * sizeof(SpriteVertexData),4 * sizeof(SpriteVertexData),&vertices[0]);
    
    sheet->renderer.vertex_count += 4;
    ++sheet->renderer.draw_count;
}

void SpriteSheetBeginRender(SpriteSheet* sheet)
{   
    BeginRender(&sheet->renderer);
    glBindTexture(GL_TEXTURE_2D, sheet->textureID);
}

void
SpriteSheetEndRender(SpriteSheet* sheet)
{
    EndRender(&sheet->renderer);
}

void 
CleanupSpriteSheet(SpriteSheet* sheet)
{
    glDeleteTextures(1, &sheet->textureID);
}

void
InitializeSpriteAnim(SpriteAnimation* anim, int count, float speed)
{
    anim->count = count;
    anim->speed = speed;
    
    anim->sprite_handles = new SpriteHandle[anim->count];
    for (int i = 0; i < anim->count; ++i)
    {
        anim->sprite_handles[i] = INVALID_SPRITE_HANDLE;
    }
}

void 
CleanupSpriteAnimation(SpriteAnimation* anim)
{
    delete[] anim->sprite_handles;
}

void
UpdateSpriteAnimation(SpriteAnimation* anim, float deltaTime)
{
    anim->timed_index += anim->speed*deltaTime;
    if (anim->timed_index > anim->count)
    {
        anim->timed_index -= anim->count;
        
        if (anim->callback != NULL)
        {
            anim->callback(anim->user_data);
        }
    }
}

void
RenderSpriteAnimationFrame(SpriteSheet* sheet, SpriteAnimation* anim, const v3f& pos)
{
    AddSpriteToRender(sheet, anim->sprite_handles[(int)anim->timed_index], pos);
}
