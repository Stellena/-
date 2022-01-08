#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Setting.h"
#define M 4

const int N = number_of_particle;
int n = elapsed_time;
int X = X_axis;
int Y = Y_axis;

typedef struct particle {
	int x;
	int y;
} particle;

particle init = { X/2, Y/2 };

int **DisperseParticles() {	

	particle ptc[N];
	particle ptcbuf;
	int i, j;
	int r;
	// num: the number of particles at (x, y)
	int **num = new int*[X];
	for (i = 0; i < X; i++) num[i] = new int[Y];

	// Initialize values of num to 0
	for (i = 0; i < X; i++) {
		for (j = 0; j < Y; j++) {
			num[i][j] = 0;
		}
	}

	// Position coordinate of particles at (1, 1)
	for (i = 0; i < N; i++) {
		ptc[i] = init;
	}

	// Disperse particles
	srand(time(NULL));

	for (i = 0; i < n; i++) {
		for (j = 0; j < N; j++) {
			
			r = rand()%M;

			switch (r) {
			case 0:
				if (ptc[j].y != 1) {
					ptc[j].y--;
				}
				break;
			case 1:
				if (ptc[j].x != X) {
					ptc[j].x++;
				}
				break;
			case 2:
				if (ptc[j].y != Y) {
					ptc[j].y++;
				}
				break;
			case 3:
				if (ptc[j].x != 1) {
					ptc[j].x--;
				}
				break;
			}

		}
	}

	// Renew num
	for (i = 0; i < N; i++) {
		num[ptc[i].x - 1][ptc[i].y - 1]++;
	}

	// The table is printed when Y <=14, since
	// the space of display is limited
	if (Y >= 1 && Y <= 14) {
		for (i = 0; i < X; i++) {
			for (j = 0; j < Y; j++) {
				// Show table
				printf("%d\t", num[i][j]);
			}
			printf("\n\n");
		}
		printf("\n");
	}
	else printf("Printing of the table is omitted because of the limited display.\n\n");

	// Prerequisites
	printf("The number of particles: %d\n", N);
	printf("The number of times particle moved: %d\n", n);
	printf("The average number of particles per unit space: %.2f\n", 
		(float) N / ((float) X * (float) Y));
	printf("\n\n");

	// Quit
	char c;
	printf("Press any key to quit the program.");
	scanf("%c", &c);

	return num;
}