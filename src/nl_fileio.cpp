#include <fstream>

void SaveGrid(Grid* grid, char* filename)
{
    std::ofstream file;
    file.open(filename);

    if (file.is_open())
    {
        file.close();
    }
}

void LoadGrid(Grid* grid, char* filename)
{
    std::ifstream file;
    file.open(filename, std::ios::bin);

    if (file.is_open())
    {
        file.close();
    }
}
