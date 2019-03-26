#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 128
#define COLS 128
#define LOGICAL_X_MIN	-4.0
#define LOGICAL_X_MAX	4.0
#define LOGICAL_Y_MIN	-4.0
#define LOGICAL_Y_MAX	4.0

void clear(unsigned char image[][COLS]);
int plot_logical_point(float x, float y, unsigned char image[][COLS]);
int plot_physical_point(int x, int y, unsigned char image[][COLS]);
int in_range(int x, int y);
void header(int row, int col, unsigned char head[32]);/*write header for the output images, do not modify this*/


int main(int argc, char **argv)
{
	int i;
	float t, x, y, z, xprime, yprime;
	FILE *fp;
	char *ofile;
	unsigned char image[ROWS][COLS];
	unsigned char head[32];
	clear(image);

	for (t = 0.01; t < 10000.01; t += .01)  //change the t range
	{
		/* The code for the other questions are in the submitted PDF file. */

		/*Modify the coefficiences according to questions*/
		float b = 1; /*b = 0, 1, -1*/
		float c = 1.0; /*c = 1, -1*/
		float x1 = -1.0;
		float x2 = 1.0;
		float y0 = -1.0;
		float z0 = 0.0;
		float fprime = 1.0;
		x = x1;
		float x_hat = x2;
		y = y0 + t*b;
		float y_hat = y0 + t*b;
		z = z0 + t*c;
		float z_hat = z0 + t*c;

		/* Modify these according to the projection */
		xprime = fprime*x/z;			
		yprime = fprime*y/z;
		float xprime_hat = fprime*x_hat/z;
		float yprime_hat = fprime*y_hat/z_hat;
		plot_logical_point(xprime, yprime, image);
		plot_logical_point(xprime_hat, yprime_hat, image);
	}
	//_CRT_SECURE_NO_WARNINGS
	/* Create a header */ 
	header(ROWS, COLS, head);

	ofile = "hw1num3.ras";// this is the name of your output file, modify the name according to the requirements

	if (!(fp = fopen(ofile, "wb")))
		fprintf(stderr, "error: could not open %s\n", ofile), exit(1);

	/* Write the header */
	fwrite(head, 4, 8, fp);

	for (i = 0; i < ROWS; i++) fwrite(image[i], 1, COLS, fp);
	fclose(fp);

	return 0;
}

void clear(unsigned char image[][COLS])
{
	int	i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) image[i][j] = 0;
}

int plot_logical_point(float x, float y, unsigned char image[][COLS])
{
	int	nx, ny;
	float	xc, yc;
	xc = COLS / ((float)LOGICAL_X_MAX - LOGICAL_X_MIN);
	yc = ROWS / ((float)LOGICAL_Y_MAX - LOGICAL_Y_MIN);
	nx = (x - LOGICAL_X_MIN) * xc;
	ny = (y - LOGICAL_Y_MIN) * yc;
	return plot_physical_point(nx, ny, image);
}

int plot_physical_point(int x, int y, unsigned char image[][COLS])
{
	if (in_range(x, y)) //return 0;
	return image[y][x] = 255;
}
int in_range(int x, int y)
{
	return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void header(int row, int col, unsigned char head[32])
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify the contents */
	
	/* Little-endian for PC */
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch++;
	head[6] = *ch;
	ch++;
	head[5] = *ch;
	ch++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch++;
	head[10] = *ch;
	ch++;
	head[9] = *ch;
	ch++;
	head[8] = *ch;

	ch = (char*)&num;
	head[19] = *ch;
	ch++;
	head[18] = *ch;
	ch++;
	head[17] = *ch;
	ch++;
	head[16] = *ch;

	/*
	// Big-endian for unix
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
	*/
}