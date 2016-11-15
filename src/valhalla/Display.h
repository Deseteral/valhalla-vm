#pragma once

#include "data_types.h"

class Display
{
public:
    Display(uint width, uint height);
    ~Display();

    u8* buffer;
};
