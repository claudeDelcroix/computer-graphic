#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <math.h>

typedef struct tagCOLOR {
	unsigned char RED;
	unsigned char GREEN;
	unsigned char BLUE;
	unsigned char ALPHA;

	tagCOLOR() : RED(0), GREEN(0), BLUE(0), ALPHA(255) {}
	tagCOLOR(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : RED(r), GREEN(g), BLUE(b), ALPHA(a) {}
} COLOR;

template<typename TYPE> void swap(TYPE& a, TYPE& b) {
	TYPE temp = a;
	a = b;
	b = temp;
}

class Frame {
private:
	COLOR* pixels;
	COLOR** matrix;

public:
	int width;
	int height;

	Frame(int _width, int _height) : width(_width), height(_height) {
		pixels = new COLOR[width * height];
		matrix = new COLOR * [height];
		for (int y = 0; y < height; y++) {
			matrix[y] = pixels + (y * width);
		}
	}

	void SetPixel(int x, int y, COLOR color) {
		if (x < 0 || x >= width || y < 0 || y >= height) return;
		matrix[y][x] = color;
	}

	COLOR GetPixel(int x, int y) {
		if (x < 0 || x >= width || y < 0 || y >= height) return COLOR(0, 0, 0, 255);
		return matrix[y][x];
	}

	void Circle(int x0, int y0, int radius, COLOR color) {
		int x = radius;
		int y = 0;
		int err = 0;
		while (x < y)
		{
			int D1 = x * x * +y * y - radius * radius;
			int D2 = x * x + (y - 1) * (y - 1) - radius * radius;

			if (D1 > -D2) y--;

			SetPixel(x0 + x, y0 + y, color);
			SetPixel(x0 + x, y0 - y, color);
			SetPixel(x0 + y, y0 + x, color);
			SetPixel(x0 + y, y0 - x, color);
			SetPixel(x0 - x, y0 + y, color);
			SetPixel(x0 - x, y0 - y, color);
			SetPixel(x0 - y, y0 + x, color);
			SetPixel(x0 - y, y0 - x, color);
			x++;
		}
	}

	void DrawLine(int x1, int y1, int x2, int y2, COLOR color) {
		int dy = y2 - y1, dx = x2 - x1;
		if (dx == 0 && dy == 0) {
			matrix[y1][x1] = color;
			return;
		}
		if (abs(dx) > abs(dy)) {
			if (x2 < x1) {
				swap(x1, x2);
				swap(y1, y2);
				dy = -dy;
				dx = -dx;
			}

			int y, dx2 = dx / 2, p = 0;
			if (dy < 0)dx2 = -dx2;
			for (int x = x1; x <= x2; x++) {
				y = (p + dx2) / dx + y1;
				p += dy;
				matrix[y][x] = color;
			}
		}
		else {
			if (y2 < y1) {
				swap(x1, x2);
				swap(y1, y2);
				dy = -dy;
				dx = -dx;
			}
			int x, dy2 = dy / 2, p = 0;
			if (dx < 0)dy2 = -dy2;
			for (int y = y1; y <= y2; y++) {
				x = (p + dy2) / dy + x1;
				p += dx;
				matrix[y][x] = color;
			}
		}
	}
	~Frame() {
		if (pixels) delete[] pixels;
		if (matrix) delete[] matrix;
	}
};

#endif // !FRAME_H

