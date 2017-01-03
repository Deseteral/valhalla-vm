#pragma once

#include "../data_types.h"

enum DisplayMode {
    TEXT, GRAPHICS
};

class Display
{
public:
    Display(uint w, uint h);
    ~Display();

    uint width;
    uint height;
    uint bufferSize;

    DisplayMode mode;
    u8* buffer;
};
