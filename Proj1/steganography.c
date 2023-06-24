/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Marco OsaOmagbon - Implementation
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *newColor = (Color*) malloc(sizeof(Color));
	// get the images Blue value
	int blue_val = image->image[row][col].B;
	if (blue_val % 2 == 0) {
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
	} else {
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
	}
	return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image* newImage = (Image*) malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color**) malloc(newImage->rows * sizeof(Color*));
	for (int i = 0; i < newImage->rows; i++) {
		newImage->image[i] = (Color *) malloc(newImage->cols * sizeof(Color));
	}
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color *newColor = evaluateOnePixel(image, i, j);
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	char *filename = argv[1];
	Image *image = readData(filename);;
	Image *newImg = steganography(image);
	writeData(newImg);
	freeImage(image);
	freeImage(newImg);
}
