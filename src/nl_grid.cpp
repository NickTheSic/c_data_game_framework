#include <nl_grid.h>
#include <nl_debug.h>

void InitGrid(Grid* grid, int width, int height)
{
    assert(width > 0 && height > 0);
    if (width == 0 || height == 0)
    {
        grid = 0;
        return;
    }

    grid->width  = width;
    grid->height = height;

    grid->data = (int*)calloc(width*height, sizeof(int));
}

void FreeGrid(Grid* grid)
{
    if (grid == 0)
    {
        return;
    }

    if (grid->data)
    {
        free(grid->data);
    }

    grid->width  = 0;
    grid->height = 0;

}
