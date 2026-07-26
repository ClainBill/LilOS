#pragma once
#include <cstdint>

struct Font
{
    uint8_t width;
    uint8_t height;

    char firstChar;
    char lastChar;

    const uint8_t* bitmap;
};