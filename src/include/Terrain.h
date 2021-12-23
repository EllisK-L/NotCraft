#pragma once
#include "Utils.h"

class Terrain{

public:
    Terrain(Utils::image& img);
    void create();
    void render();
private:

Utils::image& texture;
const Utils::point3i mapSize = {10,2,10};
Utils::point3d terrain[10][2][10];
Utils::point3f terrainC[10][2][10];


};