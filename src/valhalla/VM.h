#pragma once

#include "../data_types.h"
#include <vector>

class VM
{
public:
    VM(u16 memSize);
    ~VM();

    bool halt;

    void tick();
    void loadIntoMemory(std::vector<u8>* payload);

private:
    u16 memorySize;
    u8* memory;
    u8* registers;
    u8 pc;
};
