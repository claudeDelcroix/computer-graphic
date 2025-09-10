#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <cmath>    // std::sqrt, std::cos, std::sin
#include <cstdlib>  // std::abs

typedef struct tagCOLOR {
    unsigned char RED, GREEN, BLUE, ALPHA;
    tagCOLOR() : RED(0), GREEN(0), BLUE(0), ALPHA(255) {}
    tagCOLOR(unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a = 255)
        : RED(r), GREEN(g), BLUE(b), ALPHA(a) {
    }
} COLOR;

template<typename T>
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

class Frame {
private:
    COLOR* pixels;
    COLOR** matrix;

public:
    int width, height;

    Frame(int w, int h)
        : width(w), height(h)
    {
        pixels = new COLOR[width * height];
        matrix = new COLOR * [height];
        for (int y = 0; y < height; ++y) {
            matrix[y] = pixels + (y * width);
        }
    }

    void SetPixel(int x, int y, COLOR c) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        matrix[y][x] = c;
    }

    COLOR GetPixel(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return COLOR(0, 0, 0, 255);
        return matrix[y][x];
    }

    void Circle(int x0, int y0, int radius, COLOR c) {
        int x = 0, y = radius;
        while (x < y) {
            int D1 = x * x + y * y - radius * radius;
            int D2 = x * x + (y - 1) * (y - 1) - radius * radius;
            if (D1 > -D2) --y;
            SetPixel(x0 + x, y0 + y, c);
            SetPixel(x0 + x, y0 - y, c);
            SetPixel(x0 - x, y0 + y, c);
            SetPixel(x0 - x, y0 - y, c);
            SetPixel(x0 + y, y0 + x, c);
            SetPixel(x0 + y, y0 - x, c);
            SetPixel(x0 - y, y0 + x, c);
            SetPixel(x0 - y, y0 - x, c);
            ++x;
        }
    }

    void DrawLine(int x1, int y1, int x2, int y2, COLOR c) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        if (dx == 0 && dy == 0) {
            SetPixel(x1, y1, c);
            return;
        }
        if (std::abs(dx) > std::abs(dy)) {
            if (x2 < x1) {
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx; dy = -dy;
            }
            int err = dx / 2, sy = (dy > 0 ? 1 : -1), y = y1;
            for (int x = x1; x <= x2; ++x) {
                SetPixel(x, y, c);
                err -= std::abs(dy);
                if (err < 0) {
                    y += sy;
                    err += dx;
                }
            }
        }
        else {
            if (y2 < y1) {
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx; dy = -dy;
            }
            int err = std::abs(dy) / 2, sx = (dx > 0 ? 1 : -1), x = x1;
            for (int y = y1; y <= y2; ++y) {
                SetPixel(x, y, c);
                err -= std::abs(dx);
                if (err < 0) {
                    x += sx;
                    err += std::abs(dy);
                }
            }
        }
    }

    void DrawTriangle(int x1, int y1,
        int x2, int y2,
        int x3, int y3,
        COLOR c)
    {
        DrawLine(x1, y1, x2, y2, c);
        DrawLine(x2, y2, x3, y3, c);
        DrawLine(x3, y3, x1, y1, c);
    }

    void DrawTrianglesInCircle(int x0, int y0,
        int radius,
        int count,
        int triHeight,
        COLOR c,
        double startAngle)
    {
        const double PI = 3.141592653589793;
        double step = 2 * PI / count;
        double halfB = triHeight / std::sqrt(3.0);

        for (int i = 0; i < count; ++i) {
            //double ang = i * step; avec cette ligne le resultat est statique 
            double ang = startAngle + i * step;
            double dx = std::cos(ang);
            double dy = std::sin(ang);

            int sx = int(x0 + radius * dx);
            int sy = int(y0 + radius * dy);

            double cx = x0 + (radius - triHeight) * dx;
            double cy = y0 + (radius - triHeight) * dy;

            double px = -dy, py = dx;

            int bx1 = int(cx + halfB * px);
            int by1 = int(cy + halfB * py);
            int bx2 = int(cx - halfB * px);
            int by2 = int(cy - halfB * py);

            DrawTriangle(sx, sy, bx1, by1, bx2, by2, c);
        }
    }

    ~Frame() {
        delete[] pixels;
        delete[] matrix;
    }
};

#endif // FRAME_H
