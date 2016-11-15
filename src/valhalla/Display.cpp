#include "Display.h"

Display::Display(uint width, uint height)
{
    this->buffer = new u8[width * height];
}

Display::~Display()
{
    delete this->buffer;
}
