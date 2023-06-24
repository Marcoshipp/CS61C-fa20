/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Marco OsaOmagbon - Implementation
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	// initialize a file pointer that points to the file;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }
	// initialize the Image that we're going to return in the end;
	Image* data = (Image*) malloc(sizeof(Image));
	// skip the first line
	fscanf(fp, "%*[^\n]%*c");
	// read the next 2 lines
	int cols, rows;
	if (fscanf(fp, "%d %d", &cols, &rows) == 2) {
		data->cols = cols;
		data->rows = rows;
    } else {
		return NULL;
	}
	// skip 3rd line
	int* size = malloc(sizeof(int));
	fscanf(fp, "%d", size);
	free(size);
	// initialize a 2d array to store colors
	Color** color = (Color **) malloc(rows * sizeof(Color *));
	for (int i = 0; i < rows; i++) {
        color[i] = (Color *) malloc(cols * sizeof(Color));
    }
	for (int i = 0; i < rows; i++) {
		char line[3000];
		int numbers[cols * 3];
		// read it as a line first
		fscanf(fp, " %[^\n]", line);
		int count = 0;
		char *token = strtok(line, " \t\n");
		while (token != NULL) {
            numbers[count] = atoi(token);
            count++;
            token = strtok(NULL, " \t\n");
        }
		for (int j = 0; j < cols * 3; j += 3) {
			Color c;
			c.R = numbers[j];
			c.G = numbers[j + 1];
			c.B = numbers[j + 2];
			color[i][j / 3] = c;
		}
	}
	data->image = color;
	fclose(fp);
	return data;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			if (j == image->cols - 1) {
				printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
				break;
			}
			printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			printf("   ");
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}