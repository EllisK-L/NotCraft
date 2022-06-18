#include "include/Terrain.h"
#include "GL/freeglut.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

Terrain::Terrain(Utils::image& img, const Camera& camera) : 
    texture(img),
    cam(camera){
    //creating each chunk (will do this dynamically later)
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            for(int m = 0; m < CHUNK_HEIGHT; m++){
                for(int n = 0; n < CHUNK_WIDTH; n++){
                    for(int o = 0; o < CHUNK_WIDTH; o++){
                        BlockType newBlock;
                        if(m >= 4){
                            newBlock = bType_air;
                        }
                        else{
                            newBlock = bType_grass;
                        }
                        chunks[i][j].blocks[o][m][n].type = newBlock;
                    }
                }
            }
        }
    }
}

// Not used
void Terrain::initChunk(Chunk* chunk){
    for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                BlockType newBlock;
                if(i >= 4){
                    newBlock = bType_air;
                }
                else{
                    newBlock = bType_grass;
                }
                chunk->blocks[i][j][k] = Block{ bType_grass };
            }
        }
    }
}

void Terrain::create(){

}

bool Terrain::shouldChunkBeLoaded(Utils::point2f offset){
    bool ret = false;
    if(pow(pow((offset.x + CHUNK_WIDTH / 2) - cam.getCurrentPos().x,2)+pow((offset.y + CHUNK_WIDTH / 2) - cam.getCurrentPos().z,2), .5) <= RENDER_DISTANCE * CHUNK_WIDTH){
        ret  = true;
    }
    return ret;
}


void Terrain::render(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture.textID);
    glPushMatrix();

        //drawing a cube for at each point
        glBegin(GL_QUADS);
        glPointSize(2);
        //drawing bottom
        for(int i = 0; i < MAP_SIZE; i++){
            for(int j=0; j < MAP_SIZE; j++){
                Utils::point2f offset = {(float)i * CHUNK_WIDTH, (float)j * CHUNK_WIDTH}; // <- why float?
                if(shouldChunkBeLoaded(offset))
                    renderChunk(chunks[i][j], offset);
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Terrain::renderChunk(Chunk& chunk, Utils::point2f offset){
    for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                if(chunk.blocks[k][i][j].type != bType_air){
                    renderBlock(chunk.blocks[k][i][j], Utils::point3f({offset.x + k, (float)i, offset.y +j}));
                }
            } 
        }
    }
}


Block& Terrain::getBlockAt(Utils::point3f position){
    int chunkX = floor(position.x / CHUNK_WIDTH);
    int chunkZ = floor(position.z / CHUNK_WIDTH);
    //printf("in chunk: %d, %d\n", chunkX, chunkZ);
    if(chunkX < 0 || chunkZ < 0 || position.y < 0){
        Chunk& chunk = chunks[0][0];
        return NULLBLOCK;

    }
    else{
        if(position.x < 0 || position.y < 0 || position.z < 0){
            printf("ERROR! somehow got a broken position in getBlockAt()\n");
        }
        Chunk& chunk = chunks[chunkX][chunkZ];
        return chunk.blocks[(int)round(position.x) % CHUNK_WIDTH][(int)round(position.y)][(int)round(position.z) % CHUNK_WIDTH];
    }
}

void Terrain::generateVBO(Chunk& chunk){
    // should probably change this size to everything that isn't air to keep memory low
    Utils::point3f vertecies[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

     for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                if(chunk.blocks[k][i][j].type != bType_air){
                    // vertecies[]
                    // renderBlock(chunk.blocks[k][i][j], Utils::point3f({offset.x + k, (float)i, offset.y +j}));
                }
            } 
        }
    }
}

void Terrain::renderBlock(Block block, Utils::point3f point){
    //random colour for each block
    //glColor3f(terrainC[i][j][k].x, terrainC[i][j][k].y, terrainC[i][j][k].z);
    glColor3f(1,1,1);
    //drawing each
    //Need to improve, this is just a sanity check to make sure it's all working

    //top
    if(getBlockAt(Utils::point3f({point.x, point.y + 1, point.z})).type == bType_air){
        glTexCoord2f(1- 200.0f/texture.width,1 -201.0f/texture.height);
        glVertex3f(point.x + BLOCK_SIZE/2, 
            point.y + BLOCK_SIZE/2, 
            point.z + BLOCK_SIZE/2);

        glTexCoord2f(1- 200.0f/texture.width,1-399.0f/texture.height);
        glVertex3f(point.x + BLOCK_SIZE/2, 
            point.y + BLOCK_SIZE/2, 
            point.z - BLOCK_SIZE/2);

        glTexCoord2f(1-398.0f/texture.width,1-399.0f/texture.height);
        glVertex3f(point.x - BLOCK_SIZE/2, 
            point.y + BLOCK_SIZE/2, 
            point.z - BLOCK_SIZE/2);

        glTexCoord2f(1-398.0f/texture.width,1-201.0f/texture.height);
        glVertex3f(point.x - BLOCK_SIZE/2, 
            point.y + BLOCK_SIZE/2, 
            point.z + BLOCK_SIZE/2);
    }
    

    //bottom
    Block blockBottom = getBlockAt(Utils::point3f({point.x, point.y - 1, point.z}));
    if(blockBottom.type == bType_air || blockBottom.type == bType_null){
    glTexCoord2f(1-800.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-600.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);
    
    glTexCoord2f(1-600.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);
    
    glTexCoord2f(1-800.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);
    }

    //right
    Block blockRight = getBlockAt(Utils::point3f({point.x + 1, point.y, point.z}));
    if(blockRight.type == bType_air || blockRight.type == bType_null){
    glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-398.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);

    glTexCoord2f(1-598.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);
    
    glTexCoord2f(1-598.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);
    }

    //left
    Block blockLeft = getBlockAt(Utils::point3f({point.x - 1, point.y, point.z}));
    if(blockLeft.type == bType_air || blockLeft.type == bType_null){
    glTexCoord2f(1-200.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);
    
    glTexCoord2f(1-0.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-0.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);

    glTexCoord2f(1-201.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);
    
    }

    //front
    Block blockFront = getBlockAt(Utils::point3f({point.x, point.y, point.z + 1}));
    if(blockFront.type == bType_air || blockFront.type == bType_null){
    glTexCoord2f(1-201.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-398.0f/texture.width, 1-1.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);

    glTexCoord2f(1-201.0f/texture.width, 1-1.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z + BLOCK_SIZE/2);
    }

    //back
    Block blockBack = getBlockAt(Utils::point3f({point.x, point.y, point.z - 1}));
    if(blockBack.type == bType_air || blockBack.type == bType_null){
    glTexCoord2f(1-397.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);

    glTexCoord2f(1-200.0f/texture.width, 1-398.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y + BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);

    glTexCoord2f(1-201.0f/texture.width, 1-599.0f/texture.height);
    glVertex3f(point.x - BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);

    glTexCoord2f(1-398.0f/texture.width, 1-599.0f/texture.height);
    glVertex3f(point.x + BLOCK_SIZE/2, 
        point.y - BLOCK_SIZE/2, 
        point.z - BLOCK_SIZE/2);
    }
    //terrain[i][j][k] = {0,0,0};
}



