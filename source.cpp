#include <iostream>
#include <cmath>
#include "BMP.h"
using namespace std;

Pixel ** create_pixels_2D(int height, int width) {
	
	Pixel **pixels = new Pixel *[height];

	for (int i = 0; i < height; i++)
		pixels[i] = new Pixel[width];

	return pixels;
}

int min(int **table, int X, int Y) {
	int min = table[0][0];
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			if (min > table[i][j]) min = table[i][j];
		}
	}
	return min;
}

int max(int **table, int X, int Y) {
	int max = table[0][0];
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			if (max < table[i][j]) max = table[i][j];
		}
	}
	return max;
}

int ShowDispersion(BMPImage *imageClass, int **table, int  X, int Y) {
	cout << "min: " << min(table, X, Y) << ", max: " << max(table, X, Y) << endl;
	imageClass->create_empty_pixels(X, Y);
	imageClass->create_empty_buffer(X, Y);
	Pixel **pixels = imageClass->buffer;

	double I;
	int m = min(table, X, Y);
	int M = max(table, X, Y);

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			I = (float)(table[i][j] - m) / (float)(M - m) * 2;		// 0 <= I <= 2
			float r_buff = 255 * I;
			float g_buff = 255 * (2 - I);
			if (I >= 0 && I <= 1) {
				pixels[X - i - 1][j].r = (uint8)r_buff;
				pixels[X - i - 1][j].g = 0xFF;
				pixels[X - i - 1][j].b = 0x00;
			}
			else if (I > 1 && I <= 2) {
				pixels[X - i - 1][j].r = 0xFF;
				pixels[X - i - 1][j].g = (uint8)g_buff;
				pixels[X - i - 1][j].b = 0x00;
			}
			else {
				cout << "ERROR!" << endl;
				return 0;
			}
		}
	}

	imageClass->apply_buffer_to_pixels();
	return 0;
}

int Expansion(BMPImage *imageClass, BMPImage *resized, int n) {
	int X = imageClass->height;
	int Y = imageClass->width;
	resized->create_empty_pixels(n * X, n * Y);
	resized->create_empty_buffer(n * X, n * Y);
	Pixel **pixels = imageClass->pixels;
	Pixel **newpixels = resized->buffer;

	int k = 0, l = 0;
	for (int i = 0; i < X; i++) {
		while (k != n) {
			for (int j = 0; j < Y; j++) {
				while (l != n) {
					newpixels[n * i + k][n * j + l] = pixels[i][j];
					l++;
				}
				l = 0;
			}
			k++;
		}
		k = 0;
	}

	resized->apply_buffer_to_pixels();
	return 0;
}
