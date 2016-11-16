#pragma once

#include "data_types.h"

enum DisplayMode {
    TEXT, GRAPHICS
};

class Display
{
public:
    Display(uint width, uint height);
    ~Display();

    DisplayMode mode;
    u8* buffer;
};
