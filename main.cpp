#include <iostream>
#include <cmath>
#include "BMP.h"
#include "option.h"
#include "Setting.h"

int main() {
	BMPImage dispersion, expanded;
	int X = X_axis;
	int Y = Y_axis;
	
	int **table = DisperseParticles();
	ShowDispersion(&dispersion, table, X, Y);
	Expansion(&dispersion, &expanded, magnification_value);		// Input magnification number
	expanded.save_bmp_image("Dispersion.bmp");
		
	return 0;
}