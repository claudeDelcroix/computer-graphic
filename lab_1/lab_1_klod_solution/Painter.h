#pragma once
#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include <cmath>

struct ClickedPixel { int X, Y; };

extern float        global_angle;
extern ClickedPixel global_clicked_pixel;

class Painter {
public:
    void Draw(Frame& frame) {
        // Fond en damier
        for (int y = 0; y < frame.height; ++y) {
            for (int x = 0; x < frame.width; ++x) {
                frame.SetPixel(x, y,
                    ((x + y) % 2 == 0)
                    ? COLOR(230, 255, 230)
                    : COLOR(200, 200, 200)
                );
            }
        }

        int W = frame.width, H = frame.height;
        float a = 7.0f / 8 * ((W < H ? W - 1 : H - 1)) / std::sqrt(2.0f);
        if (a < 1.0f) return;
        a /= 2.0f;

        struct { float x, y; } C{ W / 2.0f, H / 2.0f }, A[4] = {
            {C.x + a, C.y + a}, {C.x + a, C.y - a},
            {C.x - a, C.y - a}, {C.x - a, C.y + a}
        };

        for (int i = 0; i < 4; ++i) {
            float dx = A[i].x - C.x,
                dy = A[i].y - C.y;
            A[i].x = dx * std::cos(global_angle)
                - dy * std::sin(global_angle) + C.x;
            A[i].y = dx * std::sin(global_angle)
                + dy * std::cos(global_angle) + C.y;
        }

  /*      for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            frame.DrawLine(
                int(A[i].x + 0.5f),
                int(A[i].y + 0.5f),
                int(A[j].x + 0.5f),
                int(A[j].y + 0.5f),
                COLOR(200, 30, 45)
            );
        }*/

        
        int bigR = int(a * std::sqrt(2.0f) + 0.5f);
        int triCount = 10;
        int triH = bigR / 4;
        int smallR = bigR - triH;

        frame.Circle(int(C.x), int(C.y), bigR,
            COLOR(100, 100, 250));

        frame.DrawTrianglesInCircle(
            int(C.x), int(C.y),
            bigR, triCount, triH,
            COLOR(200, 30, 45),
            global_angle
        );

        frame.Circle(int(C.x), int(C.y), smallR - 40,
            COLOR(100, 100, 250));

        // Pixel clique
        if (global_clicked_pixel.X >= 0 &&
            global_clicked_pixel.X < W &&
            global_clicked_pixel.Y >= 0 &&
            global_clicked_pixel.Y < H)
        {
            frame.SetPixel(
                global_clicked_pixel.X,
                global_clicked_pixel.Y,
                COLOR(34, 175, 60)
            );
        }
    }
};

#endif // PAINTER_H
