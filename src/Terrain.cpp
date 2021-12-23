#include "include/Terrain.h"
#include "GL/freeglut.h"

Terrain::Terrain(){
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
            }
        }
    }
}

void Terrain::render(){
    glPushMatrix();

        //drawing a cube for at each point
        glBegin(GL_QUADS);
        glPointSize(2);
        //drawing bottom
        for(int i = 0; i < mapSize.x; i++){
            for(int j=0; j < mapSize.y; j++){
                for(int k=0; k < mapSize.z; k++){


                    //drawing each
                    //Need to improve, this is just a sanity check to make sure it's all working
                    
                    //top
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);


                    //bottom
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);


                    //right
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    //left
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    //front
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z + BLOCK_SIZE/2);

                    //back
                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x + BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);

                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y - BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);
                    
                    glVertex3f(terrain[i][j][k].x - BLOCK_SIZE/2, 
                        terrain[i][j][k].y + BLOCK_SIZE/2, 
                        terrain[i][j][k].z - BLOCK_SIZE/2);



                    //terrain[i][j][k] = {0,0,0};
                }
            }
        }
        glEnd();

    glPopMatrix();
}