#pragma once
#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include <math.h>

// Définition des variables globales
extern float global_angle = 0.0f;
extern struct {
	int X;
	int Y;
} global_clicked_pixel = { -1, -1 };

class Painter {
public:
	void Draw(Frame& frame) {
		for (int y = 0; y < frame.height; y++) {
			for (int x = 0; x < frame.width; x++) {
				if ((x + y) % 2 == 0)
					frame.SetPixel(x, y, { 230, 255, 230 });
				else
					frame.SetPixel(x, y, { 200, 200, 200 });
			}
		}

		int W = frame.width, H = frame.height;
		float a = 7.0f / 8 * ((W < H) ? W - 1 : H - 1) / sqrt(2);
		if (a < 1) return;

		float angle = global_angle;
		a = a / 2;

		struct {
			float x;
			float y;
		} C = { W / 2.0f, H / 2.0f }, A[4] = { {C.x + a,C.y + a},{C.x + a,C.y - a},{C.x - a,C.y - a},{C.x - a,C.y + a} };

		for (int i = 0; i < 4; i++) {
			float x = A[i].x - C.x;
			float y = A[i].y - C.y;
			A[i].x = C.x + (x * cos(angle) - y * sin(angle));
			A[i].y = C.y + (x * sin(angle) + y * cos(angle));
		}

		for (int i = 0; i < 4; i++) {
			int i2 = (i + 1) % 4;
			frame.DrawLine((int)A[i].x + 0.5f, (int)A[i].y + 0.5f, (int)A[i2].x + 0.5f, (int)A[i2].y + 0.5f, COLOR(200, 30, 45));
		}

		frame.Circle((int)C.x, (int)C.y, (int)(a * sqrt(2) + 0.5f), COLOR(100, 100, 250));

		if (global_clicked_pixel.X >= 0 && global_clicked_pixel.X < W && global_clicked_pixel.Y >= 0 && global_clicked_pixel.Y < H)
			frame.SetPixel(global_clicked_pixel.X, global_clicked_pixel.Y, { 34, 175, 60 });
	}
};

#endif // PAINTER_H#include "Painter.h"



