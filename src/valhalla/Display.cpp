#include "Display.h"

Display::Display(uint w, uint h) : width(w), height(h)
{
    this->mode = TEXT;
    this->bufferSize = w * h;
    this->buffer = new u8[bufferSize]();
}

Display::~Display()
{
    delete this->buffer;
}
