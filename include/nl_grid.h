#ifndef _NL_GRID_H_
#define _NL_GRID_H_

struct Grid
{
    int width;
    int height;

    // I am not sure if this should be an int type or something else.
    // I could template the grid but that takes away from the c style 
    int *data;
};

void InitGrid(Grid* grid, int width, int height);
void FreeGrid(Grid* grid);

#endif //_NL_GRID_H_