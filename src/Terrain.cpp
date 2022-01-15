#include "include/Terrain.h"
#include "GL/freeglut.h"

Terrain::Terrain(Utils::image& img) : 
    texture(img){
    //creating each chunk (will do this dynamically later)
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            // initChunk(&chunks[i][j]);
        }
    }
}

void Terrain::initChunk(Chunk* chunk){
    for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                chunk->blocks[i][j][k] = Block{ bType_grass };
            }
        }
    }
}

void Terrain::create(){

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
                renderChunk(chunks[i][j], Utils::point2f({(float)i * CHUNK_WIDTH, (float)j * CHUNK_WIDTH}));
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Terrain::renderChunk(Chunk chunk, Utils::point2f offset){
    for(int i = 0; i < CHUNK_HEIGHT; i++){
        for(int j = 0; j < CHUNK_WIDTH; j++){
            for(int k = 0; k < CHUNK_WIDTH; k++){
                renderBlock(chunk.blocks[i][j][k], Utils::point3f({offset.x + k, (float)i, offset.y +j}));
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
    //glTexCoord2i(200,201);
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
    



    //bottom
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


    //right
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

    //left
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
    


    //front
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

    //back
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
    




    //terrain[i][j][k] = {0,0,0};
}