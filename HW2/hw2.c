#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ROWS		100
#define COLUMNS		100

int main( int argc, char **argv )
{

	int			i;
	FILE			*fp;
	float			u, var;
	unsigned char	image[ROWS][COLUMNS];
  	char			*ifile, *ofile, ch;
	char			filename[4][50];

	strcpy(filename[0], "image1.raw");
	strcpy(filename[1], "image2.raw");
	strcpy(filename[2], "image3.raw");
	strcpy(filename[3], "image4.raw");

	//Assign each image name in filename to ifile here	
	/* example: ifile = filename[k]; k=0,1,2,3; a loop might be needed*/
	
	int N = 100;
	for (int k = 0; k < 4; k++) {
		ifile = filename[k];
		
		if ((fp = fopen(ifile, "rb")) == NULL)
		{
			fprintf(stderr, "error: couldn't open %s\n", ifile);
			exit(1);
		}

		for (i = 0; i < ROWS; i++)
			if (fread(image[i], 1, COLUMNS, fp) != COLUMNS)
			{
				fprintf(stderr, "error: couldn't read enough stuff\n");
				exit(1);
			}

		fclose(fp);

		u = 0;
		var = 0;
		//Calculate Mean for each image here
		for (int x = 0; x < ROWS; x++) {
			for (int y = 0; y < COLUMNS; y++) {
				u += image[x][y];
			}
		}
		u *= (1.0 / (N*N));

		//Calculate Variance for each image here
		for (int x = 0; x < ROWS; x++) {
			for (int y = 0; y < COLUMNS; y++) {
				var += (image[x][y] - u)*(image[x][y] - u);
			}
		}
		var *= (1.0 / (N*N - 1));

		//Print mean and variance for each image
		printf("%s: %f %f\n", ifile, u, var);

	}

	printf("Press any key to exit: ");
	gets(&ch);

	return 0;
}


