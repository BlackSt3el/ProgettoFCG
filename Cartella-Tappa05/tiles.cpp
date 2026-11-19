#include "tiles.h"

bool loadCsvLevel(std::string csvPath, std::vector<int>& tiles, unsigned& width, unsigned& height)
{
    // File pointer
    std::ifstream file(csvPath);
    if(!file) return false;
    
    tiles.clear();
    std::string line;

    width=height=0;

    while (std::getline(file, line))
    {
        // used for breaking words
        std::stringstream s(line);
        std::string singleTile;

        unsigned count = 0;

        // read every column data of a row and
        // store it in a string variable, 'singleTile'
        while (std::getline(s, singleTile, ','))
        {
            // add all the column data
            // of a row to a vector
            tiles.push_back(std::stoi(singleTile));
            count++;
        }
        if (count == 0)
            continue;
        if(width==0)width = count;
        height++;
    }
    
    return width > 0 && tiles.size() == width*height;
}