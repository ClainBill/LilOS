#pragma once

#include <cstring>
#include <cstdint>

void drawPixel(int x, int y, uint32_t colour);
void drawLine(int x1, int y1, int x2, int y2, uint32_t colour = 0xFFFFFFFF);
void drawRect(int x1, int y1, int x2, int y2, uint32_t colour = 0xFFFFFFFF, bool fill = false);
void drawCircle(int xc, int yc, int r, uint32_t colour = 0xFFFFFFFF, bool fill = false);