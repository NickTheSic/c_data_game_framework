#ifndef _NL_GRID_H_
#define _NL_GRID_H_

constexpr int INVALID_GRID_LOCATION = -1;

struct Grid
{
    int width;
    int height;
    int tile_size;

    int *data;
};

void InitGrid(Grid* grid, int width, int height);
void FreeGrid(Grid* grid);

void GetGridValue(Grid* grid, int index, int* value);
void GetGridValue(Grid* grid, int x, int y, int* value);

void SetGridValue(Grid* grid, int index, int value);
void SetGridValue(Grid* grid, int x, int y, int value);

void SaveGrid(Grid* grid, char* filename);
void LoadGrid(Grid* grid, char* filename);

#endif //_NL_GRID_H_