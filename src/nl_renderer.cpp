#include <nl_renderer.h>

#include <nl_gl.h>
#include <iostream>

void BeginRender(Renderer* renderer)
{
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glUseProgram(renderer->shader_program);
}

void EndRender(Renderer* renderer)
{
    glDrawElements(GL_TRIANGLES, renderer->draw_count * 6, GL_UNSIGNED_INT, 0);
    renderer->vertex_count = renderer->draw_count = 0;
}

void 
InitializeRenderer(Renderer* renderer, unsigned int BatchCount, size_t DataSize)
{
    renderer->max_vertices = BatchCount*4;
    LOG("Max Vertices: %d", renderer->max_vertices);
    unsigned int IndiceCount = BatchCount*6;
    LOG("Indice Count: %d", IndiceCount);
    
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);
    
    glBindVertexArray(renderer->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer->max_vertices * DataSize, NULL, GL_DYNAMIC_DRAW);
    
    unsigned int* indices = new unsigned int[IndiceCount];
    unsigned int offset = 0;
    for (unsigned int i = 0; i < IndiceCount; i+=6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiceCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    delete[] indices;
}

void 
CleanupRenderer(Renderer* renderer)
{
    glDeleteBuffers(1, &renderer->ebo);
    glDeleteBuffers(1, &renderer->vbo);
    glDeleteVertexArrays(1, &renderer->vao);
    
    glDeleteProgram(renderer->shader_program);
}

void 
SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void 
EnableBlend(bool enabled)
{
    if (enabled)
    {
        // This could go somewhere else but this is where I enable it and I think I only use this one
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void
EnableDepth(bool enabled)
{
    if (enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void 
EnableCullFace(bool enabled)
{
    if (enabled)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void 
CullFace(bool front)
{
    if (front)
    {
        glCullFace(GL_FRONT);
    }
    else
    {
        glCullFace(GL_BACK);
    }
}

void
ResetScreenBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT); 
    glClear(GL_DEPTH_BUFFER_BIT);
}
