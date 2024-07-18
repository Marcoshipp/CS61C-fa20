/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Marco Osaomagbon
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
	FILE* imgFile = fopen(filename, "r");
	Image* img = malloc(sizeof(Image));
	char* ppmType = calloc(3, sizeof(char));
	int dim, rows, cols;
	fscanf(imgFile, "%s", ppmType);
	fscanf(imgFile, "%d %d", &cols, &rows);
	fscanf(imgFile, "%d", &dim);
	img->rows = rows, img->cols = cols;
	img->image = calloc(rows, sizeof(Color*));
	for (int i = 0; i < rows; i++) {
		img->image[i] = calloc(cols, sizeof(Color));
		Color c;
		for (int j = 0; j < cols; j++) {
			fscanf(imgFile, "%hhu %hhu %hhu", &c.R, &c.G, &c.B);
			img->image[i][j] = c;
		}
	}
	free(ppmType);
	fclose(imgFile);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color c = image->image[i][j];
			printf("%3hhu %3hhu %3hhu", c.R, c.G, c.B);
			printf("%s", j != image->cols - 1 ? "   ": "\n");
		}
	}
}

//Frees an image
void freeImage(Image *image) {
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}