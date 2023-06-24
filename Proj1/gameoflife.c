/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Marco OsaOmagbon - Implementation
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *newColor = (Color*) malloc(sizeof(Color));
	int isAliveR = image->image[row][col].R % 2;
	int isAliveG = image->image[row][col].G % 2;
	int isAliveB = image->image[row][col].B % 2;
	int aliveNeighborsR = 0, aliveNeighborsG = 0, aliveNeighborsB = 0;
	// take a look at the state of the 8 neighbors
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			int r = row + i;
			int c = col + j;
			// wraps around
			if (r < 0) {
				r = image->rows - 1;
			} else if (r >= image->rows) {
				r = 0;
			}
			if (c < 0) {
				c = image->cols - 1;
			} else if (c >= image->cols) {
				c = 0;
			}
			aliveNeighborsR += image->image[r][c].R % 2;
			aliveNeighborsG += image->image[r][c].G % 2;
			aliveNeighborsB += image->image[r][c].B % 2;
		}
	}
	newColor->R = (isAliveR == 1)? ((aliveNeighborsR == 2 || aliveNeighborsR == 3)? 255: 0): (aliveNeighborsR == 3)? 255: 0;
	newColor->G = (isAliveG == 1)? ((aliveNeighborsG == 2 || aliveNeighborsG == 3)? 255: 0): (aliveNeighborsG == 3)? 255: 0;
	newColor->B = (isAliveB == 1)? ((aliveNeighborsB == 2 || aliveNeighborsB == 3)? 255: 0): (aliveNeighborsB == 3)? 255: 0;
	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
			Color *newColor = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: %s filename rule\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char *filename = argv[1];
	uint32_t rule = (uint32_t) strtoul(argv[2], NULL, 16);
	Image *image = readData(filename);
	Image *newImg = life(image, rule);
	writeData(newImg);
	freeImage(image);
	freeImage(newImg);
}
