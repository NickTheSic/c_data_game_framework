/* date = October 23rd 2022 1:47 am */

#ifndef _NL_RENDERER_H
#define _NL_RENDERER_H

struct Renderer
{
    unsigned int vao, vbo, ebo;
    unsigned int shader_program;
    unsigned int vertex_count, max_vertices;
    unsigned int draw_count;
};

void InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize);
void CleanupRenderer(Renderer* renderer);
void BeginRender(Renderer* renderer);
void EndRender(Renderer* renderer);

#endif //_NL_RENDERER_H
