#include <nl_grid.h>
#include <nl_debug.h>

void InitGrid(Grid* grid, int width, int height)
{
    assert(width > 0 && height > 0);
    if (width <= 0 || height <= 0)
    {
        LOG("Width: %d or Height: %d was not above 0.", width, height);
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
        LOG("Trying to free already freed grid");
        return;
    }

    if (grid->data)
    {
        free(grid->data);
    }

    grid->width  = 0;
    grid->height = 0;

}

void GetGridValue(Grid* grid, int index, int* value)
{
    if (value == nullptr)
    { 
        assert(false);
        return; 
    } 

    if (index > grid->width * grid->height)
    {
        LOG("The index %d was greater than the size of the grid");
        *value = INVALID_GRID_LOCATION;
        return;
    }

    *value = grid->data[index];
}

void GetGridValue(Grid* grid, int x, int y, int* value)
{
    if (value == nullptr)
    { 
        assert(false);
        return; 
    } 

    if (x > grid->width || y > grid->height)
    {
        LOG("X: %d, Y: %d was not in grid width and height", x, y);
        *value = INVALID_GRID_LOCATION;
        return;
    }

    *value = grid->data[y*grid->width+x];
}
