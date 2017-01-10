#pragma once

#include <vector>
#include "../data_types.h"
#include "Display.h"

struct VMConfig
{
    u8 memorySize;

    uint displayWidth;
    uint displayHeight;
};

class VM
{
public:
    VM(VMConfig config);
    ~VM();

    bool halt;
    Display* display;

    void tick();
    void loadIntoMemory(std::vector<u8>* payload);

private:
    u8 memorySize;
    u8* memory;
    u8* registers;
    u8 pc;
    u8 cmp;
};
