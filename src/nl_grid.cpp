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
        LOG("The index %d was greater than the size of the grid", index);
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

void SetGridValue(Grid* grid, int index, int value)
{
    if (index < 0 || index > grid->width*grid->height)
    {
        LOG("Not a valid index for the grid - Cannot set the value");
        assert(false);
        return;
    }

    grid->data[index] = value;
}

void SetGridValue(Grid* grid, int x, int y, int value)
{
    if (x < 0 || x > grid->width || y < 0 || y > grid->height)
    {
        LOG("X: %d, Y: %d was not in grid width and height.  Unable to set grid value", x, y);
        return;
    }

    grid->data[y*grid->width+x] = value;
}

void SaveGrid(Grid* grid, char* filename)
{
    UNUSED(grid); UNUSED(filename);
}

void LoadGrid(Grid* grid, char* filename)
{
    UNUSED(grid); UNUSED(filename);
}