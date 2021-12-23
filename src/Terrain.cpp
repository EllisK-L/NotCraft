#include "include/Terrain.h"
#include "GL/freeglut.h"

Terrain::Terrain(Utils::image& img) : 
    texture(img){
    //making sure the map is initialized
    for(int i = 0; i < mapSize.x; i++){
        for(int j=0; j < mapSize.y; j++){
            for(int k=0; k < mapSize.z; k++){
                terrain[i][j][k] = {0,0,0};
            }
        }
    }
}

void Terrain::create(){
    //we are going to so a very simple create for now (just a qube)
    for(int i = 0; i < mapSize.x; i++){
        for(int j=0; j < mapSize.y; j++){
            for(int k=0; k < mapSize.z; k++){
                terrain[i][j][k] = {(double)i,(double)j,(double)k};
                terrainC[i][j][k] = {(float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100};
            }
        }
    }
}

void Terrain::render(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture.textID);
    glPushMatrix();

        //drawing a cube for at each point
        glBegin(GL_QUADS);
        glPointSize(2);
        //drawing bottom
        for(int i = 0; i < mapSize.x; i++){
            for(int j=0; j < mapSize.y; j++){
                for(int k=0; k < mapSize.z; k++){
                    //random colour for each block
                    //glColor3f(terrainC[i][j][k].x, terrainC[i][j][k].y, terrainC[i][j][k].z);
                    glColor3f(1,1,1);
                    //drawing each
                    //Need to improve, this is just a sanity check to make sure it's all working
                    //top
                    //glTexCoord2i(200,201);
                    glTexCoord2f(1- 200.0f/texture.width,1 -201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1- 200.0f/texture.width,1-399.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width,1-399.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width,1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);
                    



                    //bottom
                    glTexCoord2f(1-800.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-600.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);
                    
                    glTexCoord2f(1-600.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glTexCoord2f(1-800.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);


                    //right
                    glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-598.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glTexCoord2f(1-598.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    //left
                    glTexCoord2f(1-200.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);
                    
                    glTexCoord2f(1-0.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-0.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-201.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    


                    //front
                    glTexCoord2f(1-201.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width, 1-201.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width, 1-1.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glTexCoord2f(1-201.0f/texture.width, 1-1.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    //back
                    glTexCoord2f(1-397.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-200.0f/texture.width, 1-398.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-201.0f/texture.width, 1-599.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glTexCoord2f(1-398.0f/texture.width, 1-599.0f/texture.height);
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    




                    //terrain[i][j][k] = {0,0,0};
                }
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}