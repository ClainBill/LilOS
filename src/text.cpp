#include "text.h"
#include "main.h"
#include "fonts.h"
#include "graphics.h"

#include <string_view>

void printChar(int x, int y, char output_char, const Font& font, uint32_t colour){
    for (int j = 0; j < font.height; j++){
        uint32_t bits = font.bitmap[(output_char - font.firstChar) * font.height + j];
        for (int i = 0; i < font.width; i++){
            if (bits & (1 << (font.width - 1 - i)))
                framebuffer[(y + j) * WIDTH + x + i] = colour;
        }
    }
}

void printString(int x, int y, std::string_view text, const Font& font, uint32_t colour){
    int cursorX = x;
    int cursorY = y;
    for (char c : text){
        printChar(cursorX, cursorY, c, font, colour);
        cursorX = (cursorX + 1 + TI83Font5x7.width) % (WIDTH - TI83Font5x7.width);
        cursorY += (cursorX > WIDTH - TI83Font5x7.width - 6) ? font.height : 0;
    }
}
void debugDrawCharSheet(int x, int y){
    for (int i = TI83Font5x7.firstChar; i < TI83Font5x7.lastChar; i++){
        printChar(x, y, i, TI83Font5x7);
        if (x >= 320 - TI83Font5x7.width - 8){
            y += TI83Font5x7.height;
        }
        x = ((x + 1 + TI83Font5x7.width) % (WIDTH - TI83Font5x7.width));
    }
}

void increaseCursor(int& x, int& y, const Font& font){
    x = ((x + 1 + font.width) % (WIDTH - TI83Font5x7.width));
    if (x >= 320 - font.width - 8){
            y += font.height + 1;
        }
}

void decreaseCursor(int& x, int& y, const Font& font){
    x = ((x - 1 - font.width) % (WIDTH - TI83Font5x7.width));
    if (x < 0){
            y -= font.height + 1;
        }
}