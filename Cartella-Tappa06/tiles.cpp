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

//computes height of collision for slope tile at position tx,ty for player at position xPos, xpos must be between 0 and 16 and return a maximum height of 16 when the position is the edge of the slope
float slopeTileHeight(TileType t, int tx, int ty, float xPos){
    float localTilexPos = (xPos-tx*16.f)*0.5f;//slopes go from 0 to 8 pixels of height in 16 pixels of width 
    float topOfTile = ty*16.f;
    switch (t)
    {
        case TileType::SlopeRightBottom: case TileType::CeilLeftBottom: return topOfTile+16.f-localTilexPos;
        
        case TileType::SlopeLeftBottom:  case TileType::CeilRightBottom:  return topOfTile+localTilexPos+8.f;
        
        case TileType::SlopeRightTop:    case TileType::CeilLeftTop:    return topOfTile+8.f-localTilexPos;
        
        case TileType::SlopeLeftTop:     case TileType::CeilRightTop:    return topOfTile+(localTilexPos);
        
        default: return 0.f;
    }
}