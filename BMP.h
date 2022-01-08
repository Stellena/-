#pragma once
#include <iostream>
#include <cmath>
using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef struct {
	uint8 dummy[2];
	uint8 bfType[2];	//BMP파일 식별을 위한 Magic Number : 0x42, 0x4D
	uint32 bfSize;	//BMP파일 크기
	uint16 bfReserved[2];	//준비. 그림을 그린 응용 프로그램에 따라 다름.
	uint32 bfOffBits;	//Offset. 비트맵 데이터를 찾을 수 있는 시작	
} BMPFileHeader;

typedef struct {
	uint32 biSize; //현 구조체의 크기
	int32 biWidth; //가로
	int32 biHeight; //세로
	uint16 biPlanes; //사용하는 color plane. 기본 값 : 1
	uint16 biBitCount; //한 화소에 들어가는 비트수. 8bit * RGB = 24
	uint32 biCompression;  //압축 방식
	uint32 biSizeImage; //압축되지 않은 비트맵 데이터의 크기
	int32 biXPelsPerMeter; //그림의 해상도
	int32 biYPelsPerMeter; // 그림의 해상도
	uint32 biClrUsed; //색 팔레트의 색 수, 또는 0에서 기본 값 2^n
	uint32 biClrImportant; //중요한 색의 수. 색이 모두 중요할 경우 0. 일반적으로 무시.
}BMPInfoHeader;

typedef struct {
	BMPFileHeader fileHeader;
	BMPInfoHeader infoHeader;
} BMPHeader;

typedef struct {
	uint8 b;
	uint8 g;
	uint8 r;
} Pixel;

Pixel ** create_pixels_2D(int height, int width);

class BMPImage {
public:
	BMPHeader header;
	Pixel ** pixels;
	Pixel ** buffer;
	int height;
	int width;

	BMPImage() {
		this->height = 0;
		this->width = 0;
		this->pixels = NULL;
		this->buffer = NULL;
		set_header("dummy.bmp");
	}
	~BMPImage() {}

	void create_empty_pixels(int height, int width) {
		this->height = height;
		this->width = width;
		this->pixels = create_pixels_2D(height, width);
		set_header("dummy.bmp");
		cout << "create empty_pixel" << endl;
	}

	int create_empty_buffer(int height, int width) {
		if (this->pixels == NULL) {
			cout << "'pixels' is NULL. Cannot Create 'buffer'." << endl;
			this->width = 0;
			this->height = 0;
			return 1;
		}
		else if (this->height == 0 || this->width == 0) {
			cout << "No Width/Height Data." << endl;
			cout << "Class Crushed. Set pixels, width, height to default values NULL, 0, 0" << endl;
			cout << "Cannot conduct memory releasing. Memory leaking might happened." << endl;
			delete[] this->pixels;
			this->pixels = NULL;
			this->width = 0;
			this->height = 0;

			return 2;
		}
		else {
			if (this->buffer != NULL) {
				for (int i = 0; i < this->height; i++) {
					delete[] this->buffer[i];
				}
				delete[] this->buffer;
				this->buffer = NULL;
			}
			this->buffer = create_pixels_2D(height, width);

			return 0;
		}
	}

	// pixels를 buffer의 정보로 갱신
	int apply_buffer_to_pixels() {
		if (this->pixels != NULL) {
			if (this->width == 0 || this->height == 0) {
				cout << "No Width/Height Data." << endl;
				cout << "Class Crushed. Set pixels, width, height to default values NULL, 0, 0" << endl;
				cout << "Cannot conduct memory releasing. Memory leaking might happened." << endl;
				delete[] this->pixels;
				this->pixels = NULL;
				this->width = 0;
				this->height = 0;
			}
			else {
				for (int h = 0; h < this->height; h++) {
					delete[] this->pixels[h];
				}
				delete[] this->pixels;
				this->pixels = NULL;
			}
		}
		this->pixels = this->buffer;
		this->buffer = NULL;

		return 0;
	}

	// fName.bmp를 불러온 후, 헤더 정보를 header에,
	// pixel 정보를 pixels에 저장
	int load_bmp_image(const char * fName) {
		//만약 pixels에 정보가 담겨있다면 memory release를 한다.
		if (this->pixels != NULL) {
			for (int h = 0; h < this->height; h++) {
				delete[] this->pixels[h];
			}
			delete[] this->pixels;
			this->pixels = NULL;
		}

		FILE * file = fopen(fName, "rb");
		fread(&(this->header.fileHeader.bfType[0]), sizeof(uint8), 54, file);
		int width = this->header.infoHeader.biWidth;
		int height = this->header.infoHeader.biHeight;
		int numPadding = (4 - ((width * 3) % 4)) % 4;
		uint8 dummy[4];
		this->pixels = create_pixels_2D(height, width);
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				fread(&this->pixels[h][w], sizeof(uint8), 3, file);
			}
			fread(dummy, sizeof(uint8), numPadding, file);
		}
		this->width = width;
		this->height = height;
		set_header("dummy.bmp");
		cout << "BMP File Successfully Loaded. < " << fName << " >" << endl;
		return 0;
	}

	// pixels에 저장된 정보를 fName.bmp로 저장
	int save_bmp_image(const char * fName) {
		if (this->pixels == NULL) {
			cout << "No Pixel Data. Cannot save bmp image <" << fName << ">." << endl;
			this->width = 0;
			this->height = 0;

			return 1;
		}
		else {
			if (this->height == 0 || this->width == 0) {
				cout << "No Width/Height Data. Cannot save bmp image <" << fName << ">." << endl;
				cout << "Class Crushed. Set pixels, width, height to default values NULL, 0, 0" << endl;
				cout << "Cannot conduct memory releasing. Memory leaking might happened." << endl;
				delete[] this->pixels;
				this->pixels = NULL;
				this->width = 0;
				this->height = 0;

				return 2;
			}
			else {
				int numPadding = (4 - ((this->width * 3) % 4)) % 4;
				uint8 dummy[4] = { 0 };
				this->header.infoHeader.biSizeImage = (this->width * 3 + numPadding) * this->height;
				this->header.fileHeader.bfSize = this->header.infoHeader.biSizeImage + 54;
				this->header.infoHeader.biWidth = this->width;
				this->header.infoHeader.biHeight = this->height;

				FILE * file = fopen(fName, "wb");
				fwrite(&this->header.fileHeader.bfType[0], sizeof(uint8), 54, file);
				for (int h = 0; h < height; h++) {
					for (int w = 0; w < width; w++) {
						fwrite(&(pixels[h][w]), sizeof(uint8), 3, file);
					}
					fwrite(dummy, sizeof(uint8), numPadding, file);
				}
				fclose(file);
				cout << "BMP File Successfully Generated. < " << fName << " >" << endl;

				return 0;
			}
		}
	}

	int set_header(const char* fName) {
		FILE * file = fopen(fName, "rb");
		fread(&(this->header.fileHeader.bfType[0]), sizeof(uint8), 54, file);
		fclose(file);

		int numPadding = (4 - ((this->width * 3) % 4)) % 4;
		this->header.infoHeader.biWidth = this->width;
		this->header.infoHeader.biHeight = this->height;
		this->header.infoHeader.biSizeImage = (width * 3 + numPadding) * height;
		this->header.fileHeader.bfSize = this->header.infoHeader.biSizeImage + 54;

		return 0;
	}
};
