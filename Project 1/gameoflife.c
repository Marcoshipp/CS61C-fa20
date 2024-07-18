/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Marco Osaomagbon
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdlib.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
	Color* c = malloc(sizeof(Color));
	if (c == NULL) exit(-1);
	int dx[8] = {0, 0, 1, -1, -1, 1, 1, -1};
	int dy[8] = {1, -1, 0, 0, -1, 1, -1, 1};
	int r_cnt = 0, g_cnt = 0, b_cnt = 0;
	for (int k = 0; k < 8; k++) {
		int i = row + dx[k], j = col + dy[k];
		if (i == -1) i = image->rows - 1;
		if (j == -1) j = image->cols - 1;
		if (i == image->rows) i = 0;
		if (j == image->cols) j = 0; 
		r_cnt += image->image[i][j].R == 255;
		g_cnt += image->image[i][j].G == 255;
		b_cnt += image->image[i][j].B == 255;
	}
	int r_dx = image->image[row][col].R == 255 ? 9: 0;
	int g_dx = image->image[row][col].G == 255 ? 9: 0;
	int b_dx = image->image[row][col].B == 255 ? 9: 0;
	c->R = rule & (1 << (r_cnt + r_dx)) ? 255: 0;
	c->G = rule & (1 << (g_cnt + g_dx)) ? 255: 0;
	c->B = rule & (1 << (b_cnt + b_dx)) ? 255: 0;
	return c;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule) {
	Image* next = malloc(sizeof(Image));
	int rows = image->rows, cols = image->cols;
	next->rows = rows, next->cols = cols;
	next->image = calloc(rows, sizeof(Color*));
	for (int i = 0; i < rows; i++) {
		next->image[i] = calloc(cols, sizeof(Color));
		for (int j = 0; j < cols; j++) {
			Color* c = evaluateOneCell(image, i, j, rule);
			next->image[i][j] = *c;
			free(c);
		}
	}
	return next;
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
int main(int argc, char **argv) {
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char* filename = argv[1];
	char* hexaRule = argv[2];
	uint32_t rule = strtol(hexaRule, NULL, 16);
	Image* original = readData(filename);
	Image* next = life(original, rule);
	writeData(next);
	freeImage(original), freeImage(next);
	return 0;
}