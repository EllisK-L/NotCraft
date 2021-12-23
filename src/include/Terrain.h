#pragma once
#include "Utils.h"

class Terrain{

public:
    Terrain();
    void create();
    void render();

private:
const Utils::point3i mapSize = {10,2,10};
Utils::point3d terrain[10][2][10];

};