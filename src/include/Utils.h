#pragma once
#include <stdio.h>
#include <GL/freeglut.h>
#include <string>

#define BLOCK_SIZE 1.0f
#define PI 3.14159265
// conversion multiplier for converting from degrees to Radians
#define DEG_TO_RAD PI/180.0

namespace Utils{

    struct point3d{
        double x;
        double y;
        double z;
    };

    struct point3f{
        float x;
        float y;
        float z;
    };

    struct point3i{
        int x;
        int y;
        int z;
    };

	struct rotation3f{
		float x;
		float y;
		float z;
	};

    //maybe make this a class later?
    struct image{
        int width;
        int height;
        GLbyte* imgData;
        GLuint textID;
    };

    //This is temp code I took from my prof
    //I'll update it later so it can load real image formats and not PPMs
    static image* loadImage(std::string fileName){
	// the ID of the image file
    image* ret = (image*)malloc(sizeof(image));
	FILE *fileID;

	// maxValue
	int  maxValue;	
		
	// total number of pixels in the image
	int  totalPixels;

	// temporary character
	char tempChar;

	// counter variable for the current pixel in the image
	int i;

	// array for reading in header information
	char headerLine[100];

	// if the original values are larger than 255
	float RGBScaling;

	// temporary variables for reading in the red, green and blue data of each pixel
	int red, green, blue;

	// open the image file for reading
	fileID = fopen(fileName.c_str(), "r");

	// read in the first header line
	//    - "%[^\n]"  matches a string of all characters not equal to the new line character ('\n')
	//    - so we are just reading everything up to the first line break
	fscanf(fileID,"%[^\n] ", headerLine);

	// make sure that the image begins with 'P3', which signifies a PPM file
	if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
	{
		printf("This is not a PPM file!\n"); 
		exit(0);
	}

	// we have a PPM file
	printf("This is a PPM file\n");

	// read in the first character of the next line
	fscanf(fileID, "%c", &tempChar);

	// while we still have comment lines (which begin with #)
	while(tempChar == '#') 
	{
		// read in the comment
		fscanf(fileID, "%[^\n] ", headerLine);

		// print the comment
		printf("%s\n", headerLine);
		
		// read in the first character of the next line
		fscanf(fileID, "%c",&tempChar);
	}

	// the last one was not a comment character '#', so we need to put it back into the file stream (undo)
	ungetc(tempChar, fileID); 

	// read in the image hieght, width and the maximum value
	fscanf(fileID, "%d %d %d", &(ret->width), &(ret->height), &maxValue);

	// print out the information about the image file
	printf("%d rows  %d columns  max value= %d\n", ret->height, ret->width, maxValue);

	// compute the total number of pixels in the image
	totalPixels = ret->width * ret->height;

	// allocate enough memory for the image  (3*) because of the RGB data
	ret->imgData = (GLbyte*)malloc(3 * sizeof(GLuint) * totalPixels);


	// determine the scaling for RGB values
	RGBScaling = 255.0 / maxValue;


	// if the maxValue is 255 then we do not need to scale the 
	//    image data values to be in the range or 0 to 255
	if (maxValue == 255) 
	{
		for(i = 0; i < totalPixels; i++) 
		{
			// read in the current pixel from the file
			fscanf(fileID,"%d %d %d",&red, &green, &blue );

			// store the red, green and blue data of the current pixel in the data array
			ret->imgData[3*totalPixels - 3*i - 3] = red;
			ret->imgData[3*totalPixels - 3*i - 2] = green;
			ret->imgData[3*totalPixels - 3*i - 1] = blue;
		}
	}
	else  // need to scale up the data values
	{
		for(i = 0; i < totalPixels; i++) 
		{
			// read in the current pixel from the file
			fscanf(fileID,"%d %d %d",&red, &green, &blue );

			// store the red, green and blue data of the current pixel in the data array
			ret->imgData[3*totalPixels - 3*i - 3] = red   * RGBScaling;
			ret->imgData[3*totalPixels - 3*i - 2] = green * RGBScaling;
			ret->imgData[3*totalPixels - 3*i - 1] = blue  * RGBScaling;
		}
	}


	// close the image file
	fclose(fileID);

    glGenTextures(1, &(ret->textID));
    glBindTexture(GL_TEXTURE_2D, (ret->textID));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ret->width, ret->height, GL_RGB, GL_UNSIGNED_BYTE, ret->imgData);


    return ret;
}



}