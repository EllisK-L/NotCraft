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

    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            printf("Generating VBO: i: %d, j: %d\n",i,j);
            generateVBO(chunks[i][j], Utils::point2f{(float)i,(float)j});
            printf("Done Generating VBO\n");
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


Block& Terrain::getBlockAt(Utils::point3f position, bool debug=false){
    int chunkX = floor(position.x / CHUNK_WIDTH);
    int chunkZ = floor(position.z / CHUNK_WIDTH);
    //printf("in chunk: %d, %d\n", chunkX, chunkZ);
    if(debug){
        printf("pos x div: %f\n",round(position.x) / (float)CHUNK_WIDTH);
        printf("pos x: %f\n",round(position.x));
    }
    if(chunkX < 0 || chunkZ < 0 || position.y < 0 || round(position.x) / (float)CHUNK_WIDTH >= MAP_SIZE || round(position.z) / (float)CHUNK_WIDTH >= MAP_SIZE){
        return NULLBLOCK;
    }
    else{
        if((position.x < 0 || position.y < 0 || position.z < 0) || (chunkZ > MAP_SIZE || chunkZ > MAP_SIZE) || (position.x / (float)CHUNK_WIDTH > MAP_SIZE || position.z / (float)CHUNK_WIDTH > MAP_SIZE)){
            printf("ERROR! somehow got a broken position in getBlockAt()\n");
            fflush(stdout);
        }
        Chunk& chunk = chunks[chunkX][chunkZ];
        
        return chunk.blocks[(int)round(position.x) % CHUNK_WIDTH][(int)round(position.y)][(int)round(position.z) % CHUNK_WIDTH];
    }
}

void Terrain::generateVBO(Chunk& chunk, Utils::point2f offset){
    // should probably change this size to everything that isn't air to keep memory low
    // float vertecies[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT * 720];
    float* vertecies = (float*)malloc(sizeof(float) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT * 72);

    int blockCounter = 0;
     for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                Block block = chunk.blocks[k][i][j];
                if(block.type != bType_air){
                    getBlockVerts(block, Utils::point3f({(float)k,(float)i,(float)j}), offset);
                    // vertecies[]
                    // renderBlock(chunk.blocks[k][i][j], Utils::point3f({offset.x + k, (float)i, offset.y +j}));
                    
                    blockCounter ++;
                }
            } 
        }
    }
}

    float* Terrain::getBlockVerts(const Block& block, Utils::point3f bOffset,Utils::point2f cOffset){
        float* arr = (float*)malloc(72 * sizeof(float));

//top
    if(getBlockAt(Utils::point3f({bOffset.x + (cOffset.x * CHUNK_WIDTH), bOffset.y + 1, bOffset.z + (cOffset.y * CHUNK_WIDTH)})).type == bType_air){
        // glTexCoord2f(1- 200.0f/texture.width,1 -201.0f/texture.height);
        arr[0] = bOffset.x + BLOCK_SIZE/2;
        arr[1] = bOffset.y + BLOCK_SIZE/2;
        arr[2] = bOffset.z + BLOCK_SIZE/2;

        // glTexCoord2f(1- 200.0f/texture.width,1-399.0f/texture.height);
        arr[3] = bOffset.x + BLOCK_SIZE/2;
        arr[4] = bOffset.y + BLOCK_SIZE/2; 
        arr[5] = bOffset.z - BLOCK_SIZE/2;

        // glTexCoord2f(1-398.0f/texture.width,1-399.0f/texture.height);
        arr[6] = bOffset.x - BLOCK_SIZE/2;
        arr[7] = bOffset.y + BLOCK_SIZE/2; 
        arr[8] = bOffset.z - BLOCK_SIZE/2;

        // glTexCoord2f(1-398.0f/texture.width,1-201.0f/texture.height);
        arr[9] = bOffset.x - BLOCK_SIZE/2;
        arr[10] = bOffset.y + BLOCK_SIZE/2; 
        arr[11] = bOffset.z + BLOCK_SIZE/2;
    }
    

    //bottom
    Block blockBottom = getBlockAt(Utils::point3f({bOffset.x + (cOffset.x * CHUNK_WIDTH), bOffset.y - 1, bOffset.z + (cOffset.y * CHUNK_WIDTH)}));
    if(blockBottom.type == bType_air || blockBottom.type == bType_null){
        // glTexCoord2f(1-800.0f/texture.width, 1-201.0f/texture.height);
        arr[12] = bOffset.x + BLOCK_SIZE/2;
        arr[13] = bOffset.y - BLOCK_SIZE/2; 
        arr[14] = bOffset.z + BLOCK_SIZE/2;

        // glTexCoord2f(1-600.0f/texture.width, 1-201.0f/texture.height);
        arr[15] = bOffset.x - BLOCK_SIZE/2; 
        arr[16] =bOffset.y - BLOCK_SIZE/2; 
        arr[17] =bOffset.z + BLOCK_SIZE/2;
        
        // glTexCoord2f(1-600.0f/texture.width, 1-398.0f/texture.height);
        arr[18] = bOffset.x - BLOCK_SIZE/2; 
        arr[19] =bOffset.y - BLOCK_SIZE/2; 
        arr[20] =bOffset.z - BLOCK_SIZE/2;
        
        // glTexCoord2f(1-800.0f/texture.width, 1-398.0f/texture.height);
        arr[21] = bOffset.x + BLOCK_SIZE/2; 
        arr[22] =bOffset.y - BLOCK_SIZE/2; 
        arr[23] =bOffset.z - BLOCK_SIZE/2;
    }

    // //right
    // Block blockRight = getBlockAt(Utils::point3f({1 + bOffset.x + (cOffset.x * CHUNK_WIDTH), bOffset.y, bOffset.z + (cOffset.y * CHUNK_WIDTH)}));
    // if(blockRight.type == bType_air || blockRight.type == bType_null){
    // // glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
    // arr[24] = bOffset.x + BLOCK_SIZE/2; 
    // arr[25] =bOffset.y + BLOCK_SIZE/2; 
    // arr[26] =bOffset.z + BLOCK_SIZE/2;

    // // glTexCoord2f(1-398.0f/texture.width, 1-398.0f/texture.height);
    // arr[27] = bOffset.x + BLOCK_SIZE/2; 
    // arr[28] =bOffset.y + BLOCK_SIZE/2;
    // arr[29] =bOffset.z - BLOCK_SIZE/2;

    // // glTexCoord2f(1-598.0f/texture.width, 1-398.0f/texture.height);
    // arr[30] = bOffset.x + BLOCK_SIZE/2; 
    // arr[31] =bOffset.y - BLOCK_SIZE/2; 
    // arr[32] =bOffset.z - BLOCK_SIZE/2;
    
    // // glTexCoord2f(1-598.0f/texture.width, 1-201.0f/texture.height);
    // arr[33] = bOffset.x + BLOCK_SIZE/2; 
    // arr[34] =bOffset.y - BLOCK_SIZE/2; 
    // arr[35] =bOffset.z + BLOCK_SIZE/2;
    // }

    //left
    Block blockLeft = getBlockAt(Utils::point3f({bOffset.x + (cOffset.x * CHUNK_WIDTH) - 1, bOffset.y, bOffset.z + (cOffset.y * CHUNK_WIDTH)}));
    if(blockLeft.type == bType_air || blockLeft.type == bType_null){
    // glTexCoord2f(1-200.0f/texture.width, 1-201.0f/texture.height);
    arr[36] = bOffset.x - BLOCK_SIZE/2; 
    arr[37] =bOffset.y + BLOCK_SIZE/2; 
    arr[38] =bOffset.z + BLOCK_SIZE/2;
    
    // glTexCoord2f(1-0.0f/texture.width, 1-201.0f/texture.height);
    arr[39] = bOffset.x - BLOCK_SIZE/2; 
    arr[40] =bOffset.y - BLOCK_SIZE/2; 
    arr[41] =bOffset.z + BLOCK_SIZE/2;

    // glTexCoord2f(1-0.0f/texture.width, 1-398.0f/texture.height);
    arr[42] = bOffset.x - BLOCK_SIZE/2; 
    arr[43] =bOffset.y - BLOCK_SIZE/2; 
    arr[44] =bOffset.z - BLOCK_SIZE/2;

    // glTexCoord2f(1-201.0f/texture.width, 1-398.0f/texture.height);
    arr[45] = bOffset.x - BLOCK_SIZE/2; 
    arr[46] =bOffset.y + BLOCK_SIZE/2; 
    arr[47] =bOffset.z - BLOCK_SIZE/2;
    
    }

    //front
    Block blockFront = getBlockAt(Utils::point3f({bOffset.x + (cOffset.x * CHUNK_WIDTH), bOffset.y, 1 + bOffset.z + (cOffset.y * CHUNK_WIDTH)}));
    if(blockFront.type == bType_air || blockFront.type == bType_null){
    // glTexCoord2f(1-201.0f/texture.width, 1-201.0f/texture.height);
    arr[48] = bOffset.x + BLOCK_SIZE/2; 
    arr[49] =bOffset.y + BLOCK_SIZE/2; 
    arr[50] =bOffset.z + BLOCK_SIZE/2;

    // glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
    arr[51] = bOffset.x - BLOCK_SIZE/2; 
    arr[52] =bOffset.y + BLOCK_SIZE/2; 
    arr[53] =bOffset.z + BLOCK_SIZE/2;

    // glTexCoord2f(1-398.0f/texture.width, 1-1.0f/texture.height);
    arr[54] = bOffset.x - BLOCK_SIZE/2; 
    arr[55] =bOffset.y - BLOCK_SIZE/2; 
    arr[56] =bOffset.z + BLOCK_SIZE/2;

    // glTexCoord2f(1-201.0f/texture.width, 1-1.0f/texture.height);
    arr[57] = bOffset.x + BLOCK_SIZE/2; 
    arr[58] =bOffset.y - BLOCK_SIZE/2; 
    arr[59] =bOffset.z + BLOCK_SIZE/2;
    }

    //back
    Block blockBack = getBlockAt(Utils::point3f({bOffset.x + (cOffset.x * CHUNK_WIDTH), bOffset.y, bOffset.z + (cOffset.y * CHUNK_WIDTH)-1}));
    if(blockBack.type == bType_air || blockBack.type == bType_null){
    // glTexCoord2f(1-397.0f/texture.width, 1-398.0f/texture.height);
    arr[60] = bOffset.x + BLOCK_SIZE/2; 
    arr[61] =bOffset.y + BLOCK_SIZE/2; 
    arr[62] =bOffset.z - BLOCK_SIZE/2;

    // glTexCoord2f(1-200.0f/texture.width, 1-398.0f/texture.height);
    arr[63] = bOffset.x - BLOCK_SIZE/2; 
    arr[64] =bOffset.y + BLOCK_SIZE/2; 
    arr[65] =bOffset.z - BLOCK_SIZE/2;

    // glTexCoord2f(1-201.0f/texture.width, 1-599.0f/texture.height);
    arr[66] = bOffset.x - BLOCK_SIZE/2; 
    arr[67] =bOffset.y - BLOCK_SIZE/2; 
    arr[68] =bOffset.z - BLOCK_SIZE/2;

    // glTexCoord2f(1-398.0f/texture.width, 1-599.0f/texture.height);
    arr[69] = bOffset.x + BLOCK_SIZE/2; 
    arr[70] =bOffset.y - BLOCK_SIZE/2; 
    arr[71] =bOffset.z - BLOCK_SIZE/2;
    }


    return arr;





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



