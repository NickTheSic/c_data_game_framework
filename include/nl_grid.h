#ifndef _NL_GRID_H_
#define _NL_GRID_H_

constexpr int INVALID_GRID_LOCATION = -1;

enum class TileType : unsigned char
{
    Ground,
    Wall,
    Liquid,
};

struct Tile
{
    TileType type;
    unsigned int sprite;
};

void InitializeTileSet(Platform* platform, Tile* tiles);
void ReloadTileSet(Platform* platform, Tile* tiles, int set);

struct Grid
{
    int width;
    int height;
    int tile_size;

    int *data;
};

void InitGrid(Grid* grid, int width, int height);
void FreeGrid(Grid* grid);

void GridGetXYFromIndex(Grid* grid, int index, int* x, int *y);

void GetGridValue(Grid* grid, int index, int* value);
void GetGridValue(Grid* grid, int x, int y, int* value);

void SetGridValue(Grid* grid, int index, int value);
void SetGridValue(Grid* grid, int x, int y, int value);

void SaveGrid(Grid* grid, char* filename);
void LoadGrid(Grid* grid, char* filename);

#endif //_NL_GRID_H_