#pragma once
#include "Utils.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 8
#define MAP_SIZE 8

enum BlockType{
    bType_grass
};

class Terrain{

public:
    Terrain(Utils::image& img);
    void create();
    void render();
    struct Block{
        BlockType type;
    };
private:
    struct Chunk{
        Block blocks[CHUNK_HEIGHT][CHUNK_HEIGHT][CHUNK_WIDTH];
    };

    void renderBlock(Block block, Utils::point3f point);
    void renderChunk(Chunk chunk, Utils::point2f offset);
    void initChunk(Chunk* chunk);

    Chunk chunks[MAP_SIZE][MAP_SIZE];
    Utils::image& texture;

};