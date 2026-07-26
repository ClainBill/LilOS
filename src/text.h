#pragma once

#include "fonts/font.h"

#include <cstdint>
#include <string_view>

void printChar(int x, int y, char output_char, const Font& font, uint32_t colour = 0xFFFFFFFF);
void printString(int x, int y, std::string_view text, const Font& font, uint32_t colour = 0xFFFFFFFF);

void debugDrawCharSheet(int x, int y);