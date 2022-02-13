#pragma once
#include "Utils.h"
#include "Camera.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 64
#define MAP_SIZE 100
#define RENDER_DISTANCE 4

enum BlockType{
    bType_grass,
    bType_air
};
enum Directon{
    dir_right,
    dir_left,
    dir_up,
    dir_down
};

struct Block{
    BlockType type;
};

class Terrain{

public:

    Terrain(Utils::image& img, const Camera& camera);
    void create();
    void render();
    
    Block& getBlockAt(Utils::point3f position);

private:
    struct Chunk{
        Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];
    };

    void renderBlock(Block block, Utils::point3f point);
    void renderChunk(Chunk& chunk, Utils::point2f offset);
    void initChunk(Chunk* chunk);
    bool shouldChunkBeLoaded(Utils::point2f offset);

    Chunk chunks[MAP_SIZE][MAP_SIZE];
    Utils::image& texture;
    const Camera& cam;
};