#pragma
#include "BMP.h"


int **DisperseParticles();

int ShowDispersion(BMPImage *imageClass, int **table, int X, int Y);

// Function that expands given image n times
int Expansion(BMPImage *imageClass, BMPImage *resized, int n);