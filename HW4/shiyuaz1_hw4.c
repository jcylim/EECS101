#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS		(int)480
#define COLUMNS		(int)640

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{

	int				i, j, k, threshold[3], max[3], x, y, s;
	FILE			*fp;
	unsigned char	image[ROWS][COLUMNS], ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS], simage[ROWS][COLUMNS], head[32];
	char			filename[6][50], ifilename[50], ch;

	strcpy( filename[0], "image1" );
	strcpy( filename[1], "image2" );
	strcpy( filename[2], "image3" );
	header ( ROWS, COLUMNS, head );

	printf( "Maximum of Gx, Gy, SGM\n" );
	for ( k = 0; k < 3; k ++)
	{
		clear( ximage );
		clear( yimage );

		/* Read in the image */
		strcpy( ifilename, filename[k] );
		if (( fp = fopen( strcat(ifilename, ".raw"), "rb" )) == NULL )
		{
		  fprintf( stderr, "error: couldn't open %s\n", ifilename );
		  exit( 1 );
		}			
		for ( i = 0; i < ROWS ; i++ )
		  if ( fread( image[i], sizeof(char), COLUMNS, fp ) != COLUMNS )
		  {
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		  }
		fclose( fp );

		/* Write .RAW to .RAS image*/
		strcpy(ifilename, filename[k]);
		if (!(fp = fopen(strcat(ifilename, ".ras"), "wb")))
		{
			fprintf(stderr, "error: could not open %s\n", ifilename);
			exit(1);
		}
		fwrite(head, 4, 8, fp);
		for (i = 0; i < ROWS; i++) fwrite(image[i], 1, COLUMNS, fp);
		fclose(fp);

		max[0] = 0; //maximum of Gx
		max[1] = 0; //maximum of Gy
		max[2] = 0; //maximum of SGM

		/* Compute Gx, Gy, SGM, find out the maximum and normalize*/
	
		int xSum, ySum, sSum=0, topLeft, topRight, left, right, top, bottom, bottomLeft, bottomRight;

		/* Finding maximum for Gx */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				if (xSum > max[0]) {
					max[0] = xSum;
				}

			}
		}

		/* Finding maximum for Gy */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				if (ySum > max[1]) {
					max[1] = ySum;
				}

			}
		}

		/* Find maximum of SGM */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				sSum = pow(xSum, 2) + pow(ySum, 2);

				if (sSum > max[2]) {
					max[2] = sSum;
				}
			}
		}

		/* Compute for Gx, Gy, and SGM */
		for (x = 1; x < COLUMNS-1; x++) {
			for (y = 1; y < ROWS-1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				sSum = pow(xSum, 2) + pow(ySum, 2);
				
				/* Normalizing gradient x, y, and SGM */
				ximage[y][x] = (xSum * 255) / max[0];
				yimage[y][x] = (ySum * 255) / max[1];
				simage[y][x] = (sSum * 255) / max[2];

			}
		}

		/* Write Gx to a new image*/
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-x.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for (i = 0; i < ROWS; i++) fwrite(ximage[i], 1, COLUMNS, fp);
	    fclose( fp );
					
		/* Write Gy to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-y.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite(head, 4, 8, fp);
		for (i = 0; i < ROWS; i++) fwrite(yimage[i], 1, COLUMNS, fp);
		fclose( fp );

		/* Write SGM to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-s.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for (i = 0; i < ROWS; i++) fwrite(simage[i], 1, COLUMNS, fp);
		fclose( fp );
		
		
		/* Compute the binary image */
		for (int x = 0; x < COLUMNS; x++) {
			for (int y = 0; y < ROWS; y++) {
				if (strcmp(filename[k], "image1") == 0) {
					if (simage[y][x] > 70) {
						simage[y][x] = 255;
					}
					else {
						simage[y][x] = 0;
					}
				}
				else if (strcmp(filename[k], "image2") == 0) {
					if (simage[y][x] > 10) {
						simage[y][x] = 255;
					}
					else {
						simage[y][x] = 0;
					}
				}
				else if (strcmp(filename[k], "image3") == 0) {
					if (simage[y][x] > 10) {
						simage[y][x] = 255;
					}
					else {
						simage[y][x] = 0;
					}
				}
			}
		}

		/* Write the binary image to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-b.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for (i = 0; i < ROWS; i++) fwrite(simage[i], 1, COLUMNS, fp);
		fclose( fp );

		printf( "%d %d %d\n", max[0], max[1], max[2] );

	}

	printf( "Press any key to exit: " );
	gets( &ch );
	return 0;
}

void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
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
