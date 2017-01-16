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

struct VMInputState
{
    bool keyState[11] = { false };
};

class VM
{
public:
    VM(VMConfig config);
    ~VM();

    bool halt;
    Display* display;
    u8* registers;
    u8* memory;

    void tick(char input);
    void loadIntoMemory(std::vector<u8>* payload);

private:
    u8 memorySize;
    u8 pc;
    u8 cmp;
    string stdInput;
};
