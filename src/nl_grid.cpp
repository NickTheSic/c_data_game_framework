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

    grid->data = (int*)calloc(grid->width*grid->height, sizeof(int));

    if (grid->data == 0)
    {
        LOG("Unable to create grid data");
        assert(false);
    }
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

void GridGetXYFromIndex(Grid* grid, int index, int* x, int *y)
{
    if (x == nullptr || y == nullptr)
    { 
        assert(false);
        return; 
    } 

    *x = index % grid->width;
    *y = index / grid->width;
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


void InitializeTileSet(Platform* platform, Tile* tiles)
{
    tiles[0].type = TileType::Ground;
    tiles[1].type = TileType::Liquid;
    tiles[2].type = TileType::Wall;

    tiles[0].sprite = LoadSprite(&platform->fw.sprite_sheet, "data/ground_tile.png");
    tiles[1].sprite = LoadSprite(&platform->fw.sprite_sheet, "data/water_tile.png");
    tiles[2].sprite = LoadSprite(&platform->fw.sprite_sheet, "data/wall_tile.png");
}

void ReloadTileSet(Platform* platform, Tile* tiles, int set)
{
    switch (set)
    {
        case 0:
        {
            ReloadSprite(&platform->fw.sprite_sheet, "data/ground_tile.png", tiles[0].sprite);
            ReloadSprite(&platform->fw.sprite_sheet, "data/water_tile.png", tiles[1].sprite);
            ReloadSprite(&platform->fw.sprite_sheet, "data/wall_tile.png", tiles[2].sprite);
            break;
        }

        // TODO: Not working due to 32x32 requirement in the spritesheet.  Come back with proper 32x32 tiles
        // case 1:
        // {
        //     ReloadSprite(&platform->fw.sprite_sheet, "data/kenney/tile_0007.png", tiles[0].sprite);
        //     ReloadSprite(&platform->fw.sprite_sheet, "data/kenney/tile_0082.png", tiles[1].sprite);
        //     ReloadSprite(&platform->fw.sprite_sheet, "data/kenney/tile_0041.png", tiles[2].sprite);
        //     break;
        // }
    }
}
